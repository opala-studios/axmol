//
//  OPAdjust.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 03/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAdjust.hpp"
#include "../Json/OPJson.hpp"
#include "../Debug/OPDebug.hpp"
//#include <Adjust/Adjust2dx.h>

USING_NS_CC;

OPAdjust* OPAdjust::shared()
{
    static auto instance = new OPAdjust();
    return instance;
}

static bool deferredDeeplinkCallbackMethod(std::string deeplink)
{
    CCLOG("\nDeferred deep link received!");
    CCLOG("\nURL: %s", deeplink.c_str());
    CCLOG("\n");

//    Adjust2dx::appWillOpenUrl(deeplink);

    return true;
}

void OPAdjust::initialize(bool idfaAuthorized)
{
    auto configPath = FileUtils::getInstance()->fullPathForFilename("config/adjust.json");
    
    rapidjson::Document document;
    auto success = OPJson::parseFile(configPath, document);
    
    if (!success)
    {
        OPDebug::warn("OPAdjust", "Adjust not initialized because config file was not found.");
        return;
    }

    _config = new OPAdjustConfig(document);

    CCASSERT(!_config->getToken().empty(), "Adjust token is not present on config file.");

    std::string environment;
    
//#if COCOS2D_DEBUG
//    environment = AdjustEnvironmentSandbox2dx;
//#else
//    environment = AdjustEnvironmentProduction2dx;
//#endif
//    
//    AdjustConfig2dx adjustConfig = AdjustConfig2dx(_config->getToken(), environment);
//    adjustConfig.setSessionSuccessCallback(OPAdjust::adjustSessionSuccess);
//    adjustConfig.setSessionFailureCallback(OPAdjust::adjustSessionFailure);
//    adjustConfig.setAllowIdfaReading(idfaAuthorized);
//    adjustConfig.setDeferredDeeplinkCallback(deferredDeeplinkCallbackMethod);
//    Adjust2dx::start(adjustConfig);
}

//void OPAdjust::adjustSessionSuccess(AdjustSessionSuccess2dx event)
//{
//    OPDebug::log("OPAdjust", "Adjust session SUCCESS!");
//}
//
//void OPAdjust::adjustSessionFailure(AdjustSessionFailure2dx event)
//{
//    OPDebug::log("OPAdjust", "Adjust session FAILURE.");
//}

OPAdjustConfig* OPAdjust::getConfig() const
{
    return _config;
}

const std::string OPAdjust::getInAppPurchaseToken() const
{
    if (_config) {
        return _config->getInAppPurchaseToken();
    }
    return "";
}
