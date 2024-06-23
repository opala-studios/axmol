//
// Created by Talles Borges  on 2020-04-02.
//

#include "OPIAPManager.hpp"

#include "../Debug/OPDebug.hpp"
#include "./Manager.h"
#include "./ManagerParser.h"
#include "../Loading/OPLoadingView.hpp"
#include "../Misc/OPEventBus.hpp"

bool OPIAPManager::initialize()
{
    if (_manager != nullptr) {
        return false;
    }

    // load settings from ini file
    _manager = opalib::iap::ManagerParser::createManager("config/iap.json");
    _manager->delegate = this;

    // start service
    if (!_manager->isStarted()) _manager->startService();

    return true;
}

void OPIAPManager::restorePurchases()
{
    if (_manager == nullptr) {
        return;
    }
    _manager->restorePurchases();
    _manager->verifySubscriptions();
}

void OPIAPManager::purchaseProduct(const std::string& productIdOrAlias)
{
    if (_manager == nullptr) {
        return;
    }

    _manager->purchase(productIdOrAlias.c_str());
}

std::vector<opalib::iap::ProductSubscription*> OPIAPManager::getProSubscriptions()
{
    std::vector<opalib::iap::ProductSubscription*> products;
    for (const auto& element : _manager->getProducts()) {
        auto pSubscription = dynamic_cast<opalib::iap::ProductSubscription*>(element.second);
        if (pSubscription != nullptr) {
            products.push_back(pSubscription);
        }
    }

    return products;
}

std::vector<opalib::iap::Product*> OPIAPManager::getProducts()
{
    std::vector<opalib::iap::Product*> products;
    for (const auto& element : _manager->getProducts()) {
        products.push_back(element.second);
    }

    return products;
}

opalib::iap::Product *OPIAPManager::getProduct(const std::string &productIdOrAlias)
{
    return _manager->getProduct(productIdOrAlias.c_str());
}

bool OPIAPManager::isActiveSubscription(const std::string& productIdOrAlias)
{
    auto product = _manager->getProductSubscription(productIdOrAlias.c_str());
    if (!product) {
        return false;
    }

    return product->isActive();
}

void OPIAPManager::onPurchaseSucceed(opalib::iap::Manager* const manager, opalib::iap::Product* const product)
{
    OPDebug::log("OPIAPManager", "purchase success");

    OPEventBus::shared()->notify(opalib::Event::PurchaseSuccess{manager, product});
}

void OPIAPManager::onPurchaseFail(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "purchase fail");
    OPEventBus::shared()->notify(opalib::Event::PurchaseFail{});
}

void OPIAPManager::onServiceStarted(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "service started");
    OPEventBus::shared()->notify(opalib::Event::ProductsReady{});
}
void OPIAPManager::onTransactionStart(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "transaction start");
    OPLoadingView::show();
    OPEventBus::shared()->notify(opalib::Event::TransactionStart{});
}
void OPIAPManager::onTransactionEnd(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "transaction end");
    OPLoadingView::hide();
    OPEventBus::shared()->notify(opalib::Event::TransactionEnd{});
}
void OPIAPManager::onRestoreSucceed(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "restore success");
    OPLoadingView::hide();
    OPEventBus::shared()->notify(opalib::Event::RestoreSuccess{});
}
void OPIAPManager::onRestoreFail(opalib::iap::Manager* const manager)
{
    OPDebug::log("OPIAPManager", "restore fail");
    OPLoadingView::hide();
    OPEventBus::shared()->notify(opalib::Event::RestoreFail{});
}
void OPIAPManager::onSubscriptionVerified(opalib::iap::Manager* const manager, opalib::iap::ProductSubscription* const product)
{
    OPDebug::log("IAPManager", "subscription verified");
    OPEventBus::shared()->notify(opalib::Event::SubscriptionVerified{product});
}
