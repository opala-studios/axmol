//
// Created by Talles Borges  on 2020-04-01.
//

#ifndef OPALIBTEMPLATE_CLASSES_IAP_PRODUCTSUBSCRIPTION_HPP_
#define OPALIBTEMPLATE_CLASSES_IAP_PRODUCTSUBSCRIPTION_HPP_

#include "Product.h"

namespace opalib {
namespace iap {
class ProductSubscription : public Product
{
public:
    ProductSubscription(const char* const productId);
    bool isActive() const;
    void setIsActive(bool isActive);

protected:
//    bool _isActive{false};
};
} // namespace iap
} // namespace opalib

#endif // OPALIBTEMPLATE_CLASSES_IAP_PRODUCTSUBSCRIPTION_HPP_
