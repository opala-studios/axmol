//
//  OPAnalytics.cpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPAnalytics.hpp"
#include "OPAnalyticsEvent.hpp"

#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPAnalytics_IOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPAnalytics_Android.hpp"
#endif

#include "../Debug/OPDebug.hpp"

OPAnalytics* OPAnalytics::shared()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static OPAnalytics* instance = new OPAnalytics_IOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static OPAnalytics* instance = new OPAnalytics_Android();
#endif
    
    return instance;
}

void OPAnalytics::sendEvent(const OPAnalyticsEvent &event)
{
    OPDebug::log("Analytics", "Send event " + event.getName());
}

void OPAnalytics::initialize()
{
    setup();
}

void OPAnalytics::setCurrentScreen(const std::string& screenName)
{
    OPAnalyticsEvent("screen_view")
    .set("screen_name", screenName)
    .send();
}