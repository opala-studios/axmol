//
//  OPAdjustConfig.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 03/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAdjustConfig.hpp"
#include "../Json/OPJson.hpp"

USING_NS_CC;

OPAdjustConfig::OPAdjustConfig(const rapidjson::Value& json)
{
    _token = OPJson::getString(json, "token");

    if (OPJson::exists(json,"in_app_purchase_token")){
        _inAppPurchaseToken = OPJson::getString(json,"in_app_purchase_token");
    }
}

const std::string OPAdjustConfig::getToken() const {
    return _token;
}

const std::string& OPAdjustConfig::getInAppPurchaseToken() const
{
    return _inAppPurchaseToken;
}
