//
//  OPNativeAlert.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#include "OPNativeAlert.hpp"
#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "./OPNativeAlert_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "./OPNativeAlert_Android.hpp"
#endif

OPNativeAlert::OPNativeAlert()
{
}

OPNativeAlert* OPNativeAlert::shared()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static OPNativeAlert* instance = new OPNativeAlert_iOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static OPNativeAlert* instance = new OPNativeAlert_Android(); 
#endif

    return instance;
}

void OPNativeAlert::show(const std::string& title, const std::string& message, SEL_AlertCompletion completion)
{
    show(title, message, "Dismiss", completion);
}

void OPNativeAlert::initialize()
{
    if (!initialized)
    {
        setup();
        initialized = true;
    }
}
