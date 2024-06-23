#include "ManagerParser.h"

//#include <boost/assert.hpp>
//#include <boost/static_assert.hpp>
#include <cocos2d.h>
#include <iostream>
#include "../Debug/OPDebug.hpp"
#include "../Json/OPJson.hpp"
#include <string>
#include "Manager.h"
#include "Product.h"
#include "ProductConsumable.h"
#include "ProductSubscription.hpp"
#include "platform.h"
USING_NS_CC;

namespace opalib {
namespace iap {

Manager* ManagerParser::createManager(const char* configFile)
{
    auto manager = new Manager();

    rapidjson::Document document;
    OPDebug::ensure(OPJson::parseFile(configFile, document), "IAP Config at config/iap.json could not be loaded.");
    OPDebug::ensure(cocos2d::FileUtils::getInstance()->isFileExist("config/adjust.json"));

    for (auto& productJSON : document["products"].GetArray()) {
        auto alias = OPJson::getString(productJSON, "alias");
        auto type = OPJson::getString(productJSON, "type");

        auto productId = OPJson::getString(productJSON, getProductIdKey().c_str());
        if (productId.empty()) {
            //            OPDebug::ensure(false, "Not found product for platform");
            continue;
        }

        Product* product = nullptr;
        if (type == "non-consumable") {
            product = new Product(productId.c_str());
        } else if (type == "consumable") {
            auto quantity = OPJson::getFloat(productJSON, "quantity");
            product = new ProductConsumable(productId.c_str(), quantity);
        } else if (type == "subs") {
            product = new ProductSubscription(productId.c_str());
        } else {
            OPDebug::ensure(false, "Not found product type");
        }
        manager->addProduct(product, alias.c_str());
    }
    return manager;
}

std::string ManagerParser::getProductIdKey()
{
    std::string flavor = opalib::utils::platform::getFlavor();
    flavor[0] = std::toupper(flavor[0]);
    return opalib::utils::platform::getName() + flavor + "Id";
}

} // namespace iap
} // namespace opalib
