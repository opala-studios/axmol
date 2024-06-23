#ifndef AVALON_PAYMENT_PRODUCT_H
#define AVALON_PAYMENT_PRODUCT_H

//#include <boost/utility.hpp>
#include <string>

namespace opalib {
namespace iap {

class Manager;

// class Product : boost::noncopyable
class Product
{
    friend class Manager;

public:
    float price;
    std::string localizedPrice;
    std::string localizedName;
    std::string localizedDescription;
    std::string currencyCode;
    std::string priceAmountMicro;
    std::string salesRegion;

    explicit Product(const char* const productId);
    virtual ~Product();

    std::string getProductId() const;

    bool canBePurchased() const;
    void purchase();

    void onHasBeenPurchased();
    bool hasBeenPurchased() const;
    virtual void consume();

protected:
    int purchasedCounter;
    Manager* manager;

private:
    const std::string productId;
};

} // namespace iap
} // namespace opalib

#endif /* AVALON_PAYMENT_PRODUCT_H */
