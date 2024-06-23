//
//  OPDateTime.cpp
//  opalib-ios
//
//  Created by Opala Studios on 03/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPDateTime.hpp"
#include <chrono>

typedef std::chrono::system_clock SysClock;
typedef std::time_t Time;

OPDateTime::OPDateTime() :
            _localtime(nullptr),
            _shared(nullptr)
{}

OPDateTime::~OPDateTime()
{
    _localtime = nullptr;
}

std::shared_ptr<OPDateTime>& OPDateTime::shared()
{
    static OPDateTime instance;
    
    if (!instance._shared)
    {
        auto shared = std::make_shared<OPDateTime>();
        shared->update();
        
        instance._shared = shared;
    }
    
    return instance._shared;
}

OPDateTime& OPDateTime::setTimePoint(const time_t time)
{
    _localtime = getLocalTime(time);
    
    return (*this);
}

int OPDateTime::get(const Flag flag) const
{
    int value = -1;

    switch (flag)
    {
        case Flag::DAY:
            value = _localtime->tm_mday;
            break;
        case Flag::MONTH:
            value = _localtime->tm_mon;
            break;
        case Flag::YEAR:
            value = _localtime->tm_year;
            break;
        case Flag::DAY_WEEK:
            value = _localtime->tm_wday;
            break;
        case Flag::DAY_YEAR:
            value = _localtime->tm_yday;
            break;
        case Flag::HOUR:
            value = _localtime->tm_hour;
            break;
        case Flag::MINUTE:
            value = _localtime->tm_min;
            break;
        case Flag::SECOND:
            value = _localtime->tm_sec;
            break;
    }

    return value;
}

time_t OPDateTime::getCurrentTimePoint() const
{
    auto now = SysClock::now();

    return SysClock::to_time_t(now);
}

void OPDateTime::update()
{
    _localtime = getLocalTime();
}

tm* OPDateTime::getLocalTime(const time_t timePoint /* = -1*/)
{
    auto time = timePoint;
    
    if (time == -1)
        time = getCurrentTimePoint();

    
    return std::localtime(&time);
}

