#import "./Backend_iOSImpl.h"

#include "../../Product.h"
#include "../../ProductSubscription.hpp"
#include "../../Manager.h"
#include "../../ManagerDelegate.h"
//#include "../../../../adjust/Adjust/AdjustAppStoreSubscription2dx.h"
//#include "../../../../adjust/Adjust/Adjust2dx.h"
//#include "AdjustEvent2dx.h"
//#include "OPAdjust.hpp"
#include <vector>

@implementation Backend_iOSImpl{
    SKReceiptRefreshRequest *_refreshReceiptRequest;
}

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    if (!manager) {
        return;
    }
    
    NSNumberFormatter* numberFormatter = [[[NSNumberFormatter alloc] init] autorelease];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    
    for (SKProduct* skProduct in response.products) {
        const char* productId = [[skProduct productIdentifier] cStringUsingEncoding:NSASCIIStringEncoding];
        opalib::iap::Product* avProduct = manager->getProduct(productId);
        if (avProduct == NULL) {
            NSLog(@"[Payment] productsRequest: Product not found on our side - productId: %s", productId);
            continue;
        }
        
        avProduct->price = [[skProduct price] floatValue];
        [numberFormatter setLocale:skProduct.priceLocale];
        avProduct->localizedPrice = [[numberFormatter stringFromNumber:skProduct.price] UTF8String];
        
        // I once got rejected because the app has crashed here. Strange, huh?
        // Afer a little investigation it's rather simple. You _DON'T_ receive
        // the name and description for the product if (!!) apple has rejected
        // your product.
        //
        // In my case the guy responsible for my review simply had a small
        // question on how to use the consumables in the game. But to get
        // in contact we me he had to flag something in the app with the
        // "Developer Action Needed" stamp. Guess what he has flagged. Right.
        // All consumables and -- after he read me explanation -- he tried the
        // app again and now it crashes all the time. Rejected. Weee. I love it.
        // -- Michael (Crashlytics ID: 519409611610bcbef3330d08)
        NSString* localizedName = [skProduct localizedTitle];
        if (localizedName != NULL) {
            avProduct->localizedName = [localizedName UTF8String];
        }
        NSString* localizedDescription = [skProduct localizedDescription];
        if (localizedDescription != NULL) {
            avProduct->localizedDescription = [localizedDescription UTF8String];
        }
        
        if (@available(iOS 10, *)){
            NSString* currencyCode = [[skProduct priceLocale] currencyCode];
            if (currencyCode != NULL){
                avProduct->currencyCode = [currencyCode UTF8String];
            }
        
            NSString* salesRegion = [[skProduct priceLocale] countryCode];
            if (salesRegion != NULL){
                avProduct->salesRegion = [salesRegion UTF8String];
            }
        }
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
              if (manager->delegate) {
                  manager->delegate->onServiceStarted(manager);
              }
              // verify subscriptions
            [self verifyReceipt];
    });
    
    for (NSString* productId in response.invalidProductIdentifiers) {
        NSLog(@"[Payment] productsRequest: Product not found on apple side - productId: %@", productId);
    }
    
    
}

#pragma mark -
#pragma mark SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
                
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
                
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
                
            case SKPaymentTransactionStatePurchasing:
                // this state is known, fine and can be ignored
                break;
                
            default:
                NSLog(@"[Payment] paymentQueue: UNHANDELED: %ld", (long)transaction.transactionState);
                break;
        }
    }
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    if (manager && manager->delegate) {
        manager->delegate->onRestoreSucceed(manager);
    }
    
    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    if (error.code != SKErrorPaymentCancelled) {
        NSLog(@"[Payment] restoreCompletedTransactions failed: %@", error.localizedDescription);
        
        if (manager && manager->delegate) {
            manager->delegate->onRestoreFail(manager);
        }
    }
    
    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
}

#pragma mark -
#pragma mark Helper


- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    // it's important to NOT CALL finishTransaction in this case! because we
    // were unable to process this transaction in the user application. apple
    // will try to deliver this transaction again.
    if (!manager) {
        transactionDepth = std::max(0, --transactionDepth);
        NSLog(@"[Payment] completeTransaction failed: no manager set");
        return;
    }
    
    const char* productId = [transaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    opalib::iap::Product* product = manager->getProduct(productId);
    if (product) {
        product->onHasBeenPurchased();
    } else {
        NSLog(@"[Payment] completeTransaction failed: invalid productId: %s", productId);
    }
    
    // roughly the same reason as the return above. this is a real transaction and
    // the product was valid a few moments ago. seems to be a bug in the
    // application? nevermind. it's important to try this transaction again
    // and NOT to discard it!
    if (product) {
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        

        auto subscription = manager->getProductSubscription(productId);
        if (subscription) {
            [self sendAdjustSubscriptionTracking:transaction withProduct:product];
            subscription->setIsActive(true);//temporarilly assumes the purchase was verified
        } else {
            [self sendAdjustPurchaseTracking:product transaction:transaction];
        }
        
        if (manager->delegate) {
            manager->delegate->onPurchaseSucceed(manager, product);
        }
    }
    
    transactionDepth = std::max(0, --transactionDepth);
    if(transactionDepth == 0){
        [self verifyReceipt];
        if(manager->delegate){
            manager->delegate->onTransactionEnd(manager);
        }
    }
}

- (void)sendAdjustSubscriptionTracking:(SKPaymentTransaction*)transaction withProduct:(opalib::iap::Product*)product
{
    auto isSubscription = manager->getProductSubscription(product->getProductId().c_str());
    if (isSubscription == NULL) {
        return;
    }
    
    
    NSURL* receiptUrl = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData* receiptData = [NSData dataWithContentsOfURL:receiptUrl];
//    NSString *receiptString = [[NSString alloc] initWithData:receiptData encoding:NSUTF8StringEncoding];
    NSString *encReceipt = [receiptData base64EncodedStringWithOptions:0];
    
//    auto subscription = AdjustAppStoreSubscription2dx(
//                                                    std::to_string(product->price),
//                                                    product->currencyCode,
//                                                    [[transaction transactionIdentifier] UTF8String],
//                                                    [encReceipt UTF8String]
//                                                    );
//    subscription.setTransactionDate([[[transaction transactionDate] description] UTF8String]);
//    subscription.setSalesRegion(product->salesRegion);
//    Adjust2dx::trackAppStoreSubscription(subscription);
}

- (void)sendAdjustPurchaseTracking:(opalib::iap::Product *)product transaction:(SKPaymentTransaction *)transaction {
//    if (!OPAdjust::shared()->getInAppPurchaseToken().empty()) {
//        AdjustEvent2dx adjustEvent = AdjustEvent2dx(OPAdjust::shared()->getInAppPurchaseToken());
//        adjustEvent.setRevenue(product->price, product->currencyCode);
//        adjustEvent.setTransactionId([[transaction transactionIdentifier] UTF8String]);
//        Adjust2dx::trackEvent(adjustEvent);
//    }
    
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    // we can return early in this case. and we're allowed to finish this
    // transaction too. why? because it's just a restoreTransaction and not
    // a important purchase transaction that must reach the users application
    // as in completeTransaction().
    if (!manager) {
        transactionDepth = std::max(0, --transactionDepth);
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        return;
    }
    
    // TODO: Check it
    // The original transiction was returning nill
    // app docs say that all restored transictions has a valid payment
    // maybe it's because the product is a subscription ?
    SKPaymentTransaction* originalTransaction = transaction.originalTransaction;
    SKPayment* payment;
    if (originalTransaction == NULL) {
        payment = transaction.payment;
    }else{
        payment = originalTransaction.payment;
    }
    const char* productId = [payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    opalib::iap::Product* product = manager->getProduct(productId);
    if (product) {
        product->onHasBeenPurchased();
    } else {
        NSLog(@"[Payment] restoreTransaction invalid productId: %s", productId);
    }
    
    if (manager->delegate && product) {
        manager->delegate->onPurchaseSucceed(manager, product);
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    bool reportFailure = true;
    switch (transaction.error.code) {
        case SKErrorPaymentCancelled:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentCancelled");
            reportFailure = false;
            break;
            
        case SKErrorUnknown:
            NSLog(@"[Payment] failedTransaction: SKErrorUnknown: %@ | %@", transaction.error.localizedDescription, transaction.error.localizedFailureReason );
            break;
            
        case SKErrorClientInvalid:
            NSLog(@"[Payment] failedTransaction: SKErrorClientInvalid");
            break;
            
        case SKErrorPaymentInvalid:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentInvalid");
            break;
            
        case SKErrorPaymentNotAllowed:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentNotAllowed");
            break;
            
        default:
            NSLog(@"[Payment] failedTransaction: UNHANDELED: %ld", (long)transaction.error.code);
            break;
    }
    
    if (manager && manager->delegate && reportFailure) {
        manager->delegate->onPurchaseFail(manager);
    }
    
    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)requestDidFinish:(SKRequest *)request
{
    if([request isKindOfClass:[SKReceiptRefreshRequest class]]){
        _refreshReceiptRequest = nil;
        [self verifyReceipt];
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    if([request isKindOfClass:[SKReceiptRefreshRequest class]]){
        _refreshReceiptRequest = nil;
    }
}

- (void) verifyReceipt
{
      NSURL* receiptUrl = [[NSBundle mainBundle] appStoreReceiptURL];
    // TODO : SKReceiptRefreshRequest ir receipt not found request a new one
    if (!receiptUrl) {
        NSLog(@"[Restore] appStoreReceiptURL not found");
        return;
    }
    
    // TODO : PUT This code on the JSON Config
    NSString* urlString = [NSString stringWithUTF8String:receiptVerificationUrl];
    NSString* receiptData = [[NSData dataWithContentsOfURL:receiptUrl] base64EncodedStringWithOptions:0];
    
    if(receiptData == NULL && _refreshReceiptRequest == NULL){
        // TODO: Verify if it's the best approach
        _refreshReceiptRequest = [[SKReceiptRefreshRequest alloc] initWithReceiptProperties:@{}];
        _refreshReceiptRequest.delegate = self;
        [_refreshReceiptRequest start];
        return;
    }
    
    NSDictionary* requestDictionary = [[NSDictionary alloc] initWithObjects:@[receiptData] forKeys:@[@"receipt-data"]];
    NSData* requestData = [NSJSONSerialization dataWithJSONObject:requestDictionary options:NSJSONWritingPrettyPrinted error:nil];
    NSMutableURLRequest* request =  [NSMutableURLRequest requestWithURL:[NSURL URLWithString:urlString]];
    [request setHTTPMethod:@"POST"];
    [request setValue:@"Application/json" forHTTPHeaderField:@"Content-Type"];
    [request setHTTPBody:requestData];
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if(!data){
            NSLog(@"[Restore] data return null");
            return;
        }
        
        NSDictionary* json = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:nil];
        if (!json) {
            NSLog(@"[Restore] Fail to parse json");
            return;
        }
        NSLog(@"%@", json);
        
        
        
        
        std::vector<std::string> activeSubscriptions;
        NSArray* receiptsArray = json[@"valid_products"];
        for(int i = 0; i < receiptsArray.count; i++) {
            NSDictionary* receipt = receiptsArray[i];
            const char* productId = [receipt[@"product_id"] cStringUsingEncoding:NSASCIIStringEncoding];
            activeSubscriptions.push_back(productId);
        }

        std::map<const std::string, opalib::iap::Product* const> products = manager->getProducts();
        for(auto const& element : products){
            auto productId = element.first;
            auto subscription = manager->getProductSubscription(productId.c_str());
            if (subscription) {
                
                subscription->setIsActive(std::count(activeSubscriptions.begin(),activeSubscriptions.end(),productId));
                if (manager->delegate) {
                    manager->delegate->onSubscriptionVerified(manager, subscription);
                }
            }
                
        }
//        opalib::payment::ProductSubscription* product = manager->getProductSubscription(productId);
//        if (product) {
//            product->setIsActive(true);
//            if (manager->delegate) {
//                manager->delegate->onPurchaseSucceed(manager, product);
//            }
//        } else {
//            NSLog(@"[Payment] restoreTransaction invalid productId: %s", productId);
//        }

        // TODO: add callback when finished
    }] resume];
}

@end
