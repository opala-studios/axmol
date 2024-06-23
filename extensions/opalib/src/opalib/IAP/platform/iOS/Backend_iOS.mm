#include "../../Backend.h"

#include <StoreKit/StoreKit.h>

#include "./Backend_iOSImpl.h"
#include "../../Manager.h"
#include "../../ManagerDelegate.h"
#include "../../Product.h"
#include "../../../Json/OPJson.hpp"
#include "../../../Debug/OPDebug.hpp"

namespace opalib {
namespace iap {

Backend_iOSImpl* const __getIosBackend()
{
    static Backend_iOSImpl* instance = nullptr;
    if (!instance) {
        instance = [[Backend_iOSImpl alloc] init];
        instance->initialized = false;
    }
    return instance;
}

Backend::Backend(Manager& manager)
: manager(manager)
{
}

Backend::~Backend()
{
    shutdown();
}

bool Backend::isInitialized() const
{
    return __getIosBackend()->initialized;
}

void Backend::initialize()
{
    // configure BackendIos
    __getIosBackend()->initialized = true;
    __getIosBackend()->manager = &manager;
    __getIosBackend()->transactionDepth = 0;

    rapidjson::Document document;
    OPDebug::ensure(OPJson::parseFile("config/iap.json", document), "IAP Config at config/iap.json could not be loaded.");
    
    std::string* receiptUrl = new std::string(document["general"]["iosReceiptVerificationUrl"].GetString());
    __getIosBackend()->receiptVerificationUrl = receiptUrl->c_str();
    
    // register transcationObserver
    [[SKPaymentQueue defaultQueue] addTransactionObserver:__getIosBackend()];

    // convert opalib::Payment::ProductList into NSMutableSet
    NSMutableSet* products = [[[NSMutableSet alloc] init] autorelease];
    for (const auto& pair : manager.getProducts()) {
        [products addObject:@(pair.second->getProductId().c_str())];
    }

    // fetch product details
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:products];
    request.delegate = __getIosBackend();
    [request start];
}

void Backend::shutdown()
{
    if (!isInitialized()) {
        return;
    }

    [[SKPaymentQueue defaultQueue] removeTransactionObserver:__getIosBackend()];
    __getIosBackend()->initialized = false;
    __getIosBackend()->manager = NULL;
}

void Backend::purchase(Product* const product)
{
    if (__getIosBackend()->transactionDepth == 0) {
        if (manager.delegate) {
            manager.delegate->onTransactionStart(&manager);
        }
    }
    __getIosBackend()->transactionDepth += 1;

    NSString* productId = [[[NSString alloc] initWithUTF8String:product->getProductId().c_str()] autorelease];
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:productId];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

bool Backend::isPurchaseReady() const
{
    return [SKPaymentQueue canMakePayments];
}

void Backend::restorePurchases() const
{
    if (__getIosBackend()->transactionDepth == 0) {
        if (manager.delegate) {
            manager.delegate->onTransactionStart(&manager);
        }
    }
    __getIosBackend()->transactionDepth += 1;

    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

void Backend::verifySubscriptions() const
{
    [__getIosBackend() verifyReceipt];
}

} // namespace payment
} // namespace opalib
