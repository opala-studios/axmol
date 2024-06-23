//
// Created by Talles Borges  on 2020-04-02.
//

#ifndef OPALIBTEMPLATE_CLASSES_IAP_OPIAPMANAGER_HPP_
#define OPALIBTEMPLATE_CLASSES_IAP_OPIAPMANAGER_HPP_

#include <map>
#include <string>

#include "./ManagerDelegate.h"
#include "./ProductSubscription.hpp"

namespace opalib {
namespace Event {
struct PurchaseSuccess
{
    opalib::iap::Manager* manager;
    opalib::iap::Product* product;
};

struct SubscriptionVerified
{
    opalib::iap::ProductSubscription* subscription;
};

struct PurchaseFail
{};
struct TransactionStart
{};

struct TransactionEnd
{};

struct RestoreSuccess
{};

struct RestoreFail
{};

struct ProductsReady
{};

} // namespace Event
} // namespace opalib

class OPIAPManager : public opalib::iap::ManagerDelegate
{
public:
    static OPIAPManager* shared()
    {
        static OPIAPManager _sharedInstance;
        return &_sharedInstance;
    }

    virtual bool initialize();

    virtual opalib::iap::Product* getProduct(const std::string& productIdOrAlias);
    virtual std::vector<opalib::iap::Product*> getProducts();
    virtual std::vector<opalib::iap::ProductSubscription*> getProSubscriptions();

    virtual void purchaseProduct(const std::string& productIdOrAlias);
    virtual bool isActiveSubscription(const std::string& productIdOrAlias);

    virtual void restorePurchases();

protected:
    opalib::iap::Manager* _manager{nullptr};

    void onPurchaseSucceed(opalib::iap::Manager* const manager, opalib::iap::Product* const product) override;
    void onPurchaseFail(opalib::iap::Manager* const manager) override;
    void onSubscriptionVerified(opalib::iap::Manager* const manager, opalib::iap::ProductSubscription* const product) override;
    void onServiceStarted(opalib::iap::Manager* const manager) override;
    void onTransactionStart(opalib::iap::Manager* const manager) override;
    void onTransactionEnd(opalib::iap::Manager* const manager) override;
    void onRestoreSucceed(opalib::iap::Manager* const manager) override;
    void onRestoreFail(opalib::iap::Manager* const manager) override;

protected:
    OPIAPManager(){};
};
#endif // OPALIBTEMPLATE_CLASSES_IAP_OPIAPMANAGER_HPP_
