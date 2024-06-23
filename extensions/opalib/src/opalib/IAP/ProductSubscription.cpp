//
// Created by Talles Borges  on 2020-04-01.
//

#include <opalib/UserData/OPUserData.hpp>
#include "ProductSubscription.hpp"

namespace opalib {
namespace iap {
ProductSubscription::ProductSubscription(const char* const productId) : Product(productId) {}

bool ProductSubscription::isActive() const
{
    return OPUserData::shared()->getBool("subscription_id_" + getProductId() + "_active");
}
void ProductSubscription::setIsActive(bool isActive)
{
    OPUserData::shared()->setBool("subscription_id_" + getProductId() + "_active", isActive);
}

} // namespace iap
} // namespace opalib
