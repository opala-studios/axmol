#ifndef AVALON_PAYMENT_PRODUCTCONSUMABLE_H
#define AVALON_PAYMENT_PRODUCTCONSUMABLE_H

#include "Product.h"

namespace opalib {
namespace iap {

class ProductConsumable : public Product
{
public:
    ProductConsumable(const char* const productId, const float quantityPerPurchase);
    virtual ~ProductConsumable();

    virtual void consume() override;
    float getQuantity() const;

protected:
    const float quantityPerPurchase;
};

} // namespace iap
} // namespace opalib

#endif /* AVALON_PAYMENT_PRODUCTCONSUMABLE_H */
