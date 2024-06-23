//
//  OPAnalyticsEvent.cpp
//  Opalib
//
//  Created by Adilson Tavares on 26/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPAnalyticsEvent.hpp"
#include "OPAnalytics.hpp"

OPAnalyticsEvent::OPAnalyticsEvent(const std::string& name)
{
    this->name = name;
}

OPAnalyticsEvent& OPAnalyticsEvent::set(const std::string& name, bool boolValue)
{
    parameters[name] = OPParameter(boolValue);
    return *this;
}

OPAnalyticsEvent& OPAnalyticsEvent::set(const std::string& name, int intValue)
{
    parameters[name] = OPParameter(intValue);
    return *this;
}

OPAnalyticsEvent& OPAnalyticsEvent::set(const std::string& name, double doubleValue)
{
    parameters[name] = OPParameter(doubleValue);
    return *this;
}

OPAnalyticsEvent& OPAnalyticsEvent::set(const std::string& name, const char* stringValue)
{
    parameters[name] = OPParameter(std::string(stringValue, strlen(stringValue)));
    return *this;
}

OPAnalyticsEvent& OPAnalyticsEvent::set(const std::string& name, const std::string& stringValue)
{
    parameters[name] = OPParameter(stringValue);
    return *this;
}

void OPAnalyticsEvent::send() const
{
    OPAnalytics::shared()->sendEvent(*this);
}