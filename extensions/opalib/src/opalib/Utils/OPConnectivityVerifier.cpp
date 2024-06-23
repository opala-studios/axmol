//
//  OPConnectivityVerifier.cpp
//  opalib-ios
//
//  Created by Opala Studios on 03/11/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPConnectivityVerifier.hpp"
#include <cocos2d.h>

bool OPConnectivityVerifier::isAirplaneModeOn()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string className = "com/opalastudios/opalib/activity/OpalibActivity";
    return cocos2d::JniHelper::callStaticBooleanMethod(className, "isAirplaneModeOn");
#endif
    
    return false;
}

bool OPConnectivityVerifier::isActiveInternetConnection()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string className = "com/opalastudios/opalib/activity/OpalibActivity";
    return cocos2d::JniHelper::callStaticBooleanMethod(className, "isActiveInternetConnection");
#endif

    return true;
}
