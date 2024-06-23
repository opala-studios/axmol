//
//  OPAnalyticsIOS.cpp
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "./OPAnalytics_IOS.hpp"
#include "./OPAnalytics_IOSImpl.h"

OPAnalytics_IOS::OPAnalytics_IOS()
{
    impl = (void*)[[OPAnalytics_IOSImpl alloc] init];
}

void OPAnalytics_IOS::setup()
{
    [((OPAnalytics_IOSImpl*)impl) setup];
}

void OPAnalytics_IOS::sendEvent(const OPAnalyticsEvent &event)
{
    OPAnalytics::sendEvent(event);
    [((OPAnalytics_IOSImpl*)impl) sendEvent:event];
}

void OPAnalytics_IOS::setUserProperty(const std::string& key, const std::string& value){
    [((OPAnalytics_IOSImpl*)impl) setUserProperty:key withValue:value];
}
