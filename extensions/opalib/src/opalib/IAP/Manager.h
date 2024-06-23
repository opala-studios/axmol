#ifndef AVALON_PAYMENT_MANAGER_H
#define AVALON_PAYMENT_MANAGER_H

//#include <boost/utility.hpp>
#include <map>
#include <string>
#include "Backend.h"

namespace opalib {
namespace iap {

class ManagerDelegate;
class Product;
class ProductConsumable;
class ProductSubscription;
typedef std::map<const std::string, Product* const> ProductList;

/**
 * Main actor for the whole payment handling and also the mose user facing part.
 *
 * - Keeps track of all available products
 * - Instantiates and holds the native payment::Backend
 * - Can store products under a second name (alias)
 * - Simple interface to start product purchase with only a string
 *
 * @warning It's a good idea to use iap::Loader to instaniate and configure
 *          this class!
 */
// class Manager : boost::noncopyable
class Manager
{
public:
    /**
     * Controls wether all iap::ProductConsumable with quantity > 0 should
     * be consumed in ~Manager() to avoid/ignore the otherwise triggered error.
     */
    bool ignoreUnusedConsumableQuantities;
    ManagerDelegate* delegate;

    Manager();
    ~Manager();

    void addProduct(Product* const product);
    void addProduct(Product* const product, const char* const alias);

    const ProductList& getProducts() const;
    Product* getProduct(const char* const productIdOrAlias) const;
    ProductConsumable* getProductConsumable(const char* const productIdOrAlias) const;
    ProductSubscription* getProductSubscription(const char* const productIdOrAlias) const;
    bool hasProduct(const char* const productIdOrAlias) const;

    void purchase(const char* const productIdOrAlias);
    bool isPurchaseReady() const;
    void restorePurchases() const;
    void verifySubscriptions() const;

    void startService();
    void stopService();
    bool isStarted() const;

    Backend& getBackend();

private:
    bool started;
    Backend backend;
    mutable ProductList products;
    std::map<std::string, std::string> productIdAliases;
};

} // namespace iap
} // namespace opalib

#endif /* AVALON_PAYMENT_MANAGER_H */
