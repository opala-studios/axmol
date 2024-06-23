#include "Manager.h"

//#include <boost/assert.hpp>
//#include <boost/cast.hpp>
#include "../Debug/OPDebug.hpp"
#include <typeinfo>
#include "Product.h"
#include "ProductConsumable.h"
#include "ProductSubscription.hpp"

// using boost::polymorphic_downcast;
using std::make_pair;
using std::string;

namespace opalib {
namespace iap {

Manager::Manager()
    : ignoreUnusedConsumableQuantities(false),
      delegate(),
      started(false),
      backend(*this),
      products(),
      productIdAliases()
{}

Manager::~Manager()
{
    for (auto& pair : products) {
        auto& product = pair.second;

        if (ignoreUnusedConsumableQuantities) {
            auto consumable = dynamic_cast<ProductConsumable* const>(product);
            if (consumable) {
                consumable->consume();
            }
        }

        delete product;
    }

    products.clear();
    productIdAliases.clear();
}

void Manager::addProduct(Product* const product)
{
    if (backend.isInitialized()) {
        OPDebug::ensure(false, "backend already initialized");
        return;
    }

    if (!product) {
        OPDebug::ensure(false, "product must be given");
        return;
    }
    OPDebug::ensure(!hasProduct(product->getProductId().c_str()), "productId already in use");

    products.insert(make_pair(product->getProductId(), product));
    product->manager = this;
}

void Manager::addProduct(Product* const product, const char* const alias)
{
    OPDebug::ensure(!hasProduct(alias), "given alias already in use");

    addProduct(product);
    productIdAliases.insert(make_pair(string(alias), product->getProductId()));
}

const ProductList& Manager::getProducts() const
{
    return products;
}

Product* Manager::getProduct(const char* const productIdOrAlias) const
{
    auto productId = string(productIdOrAlias);
    auto product = products.find(productId);
    if (product != products.end()) {
        return product->second;
    }

    if (productIdAliases.count(productId) > 0) {
        auto aliasedId = productIdAliases.at(productId);
        auto aliasedProduct = products.find(aliasedId);
        if (aliasedProduct != products.end()) {
            return aliasedProduct->second;
        }
    }

    //    OPDebug::ensure(false, "invalid productId or alias given");
    return nullptr;
}

ProductConsumable* Manager::getProductConsumable(const char* const productIdOrAlias) const
{
    return dynamic_cast<ProductConsumable*>(getProduct(productIdOrAlias));
}

ProductSubscription* Manager::getProductSubscription(const char* const productIdOrAlias) const
{
    return dynamic_cast<ProductSubscription*>(getProduct(productIdOrAlias));
}

bool Manager::hasProduct(const char* const productIdOrAlias) const
{
    auto productId = string(productIdOrAlias);

    if (products.count(productId) > 0) {
        return true;
    }

    if (productIdAliases.count(productId) == 0) {
        return false;
    }

    auto aliasId = productIdAliases.at(productId);
    return (products.count(aliasId) > 0);
}

void Manager::purchase(const char* const productIdOrAlias)
{
    if (!isPurchaseReady()) {
        OPDebug::ensure(false, "backend service not started yet");
        return;
    }

    auto product = getProduct(productIdOrAlias);
    if (product) {
        backend.purchase(product);
    }
}

void Manager::startService()
{
    if (isStarted()) {
        OPDebug::ensure(false, "service already started");
        return;
    }

    if (!delegate) {
        throw new std::runtime_error("iap delegate must be set!");
    }

    backend.initialize();
    started = true;
}

void Manager::stopService()
{
    backend.shutdown();
    started = false;
}

bool Manager::isStarted() const
{
    return started;
}

bool Manager::isPurchaseReady() const
{
    return (delegate && backend.isInitialized() && backend.isPurchaseReady());
}

void Manager::restorePurchases() const
{
    if (!isPurchaseReady()) {
        OPDebug::ensure(false, "backend service not started yet");
        return;
    }

    backend.restorePurchases();
}

Backend& Manager::getBackend()
{
    return backend;
}

void Manager::verifySubscriptions() const
{
    if (!isPurchaseReady()) {
        OPDebug::ensure(false, "backend service not started yet");
        return;
    }
    backend.verifySubscriptions();
}

} // namespace iap
} // namespace opalib
