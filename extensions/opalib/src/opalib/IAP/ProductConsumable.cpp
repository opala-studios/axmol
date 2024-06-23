#include "ProductConsumable.h"

//#include <boost/assert.hpp>

namespace opalib {
namespace iap {

ProductConsumable::ProductConsumable(const char* const productId, const float quantityPerPurchase)
    : Product(productId), quantityPerPurchase(quantityPerPurchase)
{}

ProductConsumable::~ProductConsumable()
{
    //    BOOST_ASSERT_MSG(getQuantity() == 0, "unused consumable quantity detected!");
}

void ProductConsumable::consume()
{
    Product::consume();
    purchasedCounter = 0;
}

float ProductConsumable::getQuantity() const
{
    return (purchasedCounter * quantityPerPurchase);
}

} // namespace iap
} // namespace opalib
