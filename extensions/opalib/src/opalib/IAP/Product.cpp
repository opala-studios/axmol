#include "Product.h"

//#include <boost/assert.hpp>
#include "Manager.h"

#include "UserData/OPUserData.hpp"

namespace opalib {
namespace iap {

Product::Product(const char* const productId)
    : price(0),
      localizedPrice(),
      localizedName(),
      localizedDescription(),
      purchasedCounter(0),
      manager(NULL),
      productId(std::string(productId))
{}

Product::~Product() {}

std::string Product::getProductId() const
{
    return productId;
}

bool Product::canBePurchased() const
{
    if (!manager || !manager->isPurchaseReady()) {
        return false;
    }

    return true;
}

void Product::purchase()
{
    if (!manager) {
        //        BOOST_ASSERT_MSG(false, "service has to be set");
        return;
    }

    manager->purchase(getProductId().c_str());
}

void Product::onHasBeenPurchased()
{
    int purchaseCounter = OPUserData::shared()->getInt("product_id_" + getProductId() + "_purchased");
    OPUserData::shared()->setInt("product_id_" + getProductId() + "_purchased", purchaseCounter + 1);
}

bool Product::hasBeenPurchased() const
{
    return OPUserData::shared()->getInt("product_id_" + getProductId() + "_purchased") > 0;
}

void Product::consume() {}

} // namespace iap
} // namespace opalib
