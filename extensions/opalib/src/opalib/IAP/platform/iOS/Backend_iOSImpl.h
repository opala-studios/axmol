#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#include "../../Product.h"

namespace opalib { namespace iap { class Manager;} }

@interface Backend_iOSImpl : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    @public bool initialized;
    @public opalib::iap::Manager* manager;
    @public int transactionDepth;
    @public const char* receiptVerificationUrl;
}

#pragma mark -
#pragma mark Subscription Helper
- (void) verifyReceipt;

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;

#pragma mark -
#pragma mark SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;

#pragma mark -
#pragma mark Helper

- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;
- (void)sendAdjustSubscriptionTracking:(SKPaymentTransaction*)transaction withProduct:(opalib::iap::Product*)product;

@end
