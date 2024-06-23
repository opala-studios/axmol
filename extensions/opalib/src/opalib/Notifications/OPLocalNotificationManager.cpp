//
// Created by Opala Studios on 03/09/21.
//

#include "OPLocalNotificationManager.hpp"
#include <cocos2d.h>
#include "../Utils/OPDateTime.hpp"
#include "../Misc/OPEventBus.hpp"
#include "../Events/OPEvents.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Platforms/ios/OPLocalNotificationManager_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "opalib/Notifications/Platforms/Android/OPLocalNotificationManager_Android.hpp"
#endif

OPLocalNotificationManager::OPLocalNotificationManager() :
                            _shared(nullptr),
                            _type("DEFAULT")
{}

std::shared_ptr<OPLocalNotificationManager> OPLocalNotificationManager::shared()
{
    static OPLocalNotificationManager instance;

    if (!instance._shared)
    {
        std::shared_ptr<OPLocalNotificationManager> shared = nullptr;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        shared = std::make_shared<OPLocalNotificationManager_iOS>(); 
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        shared = std::make_shared<OPLocalNotificationManager_Android>();
#endif
        
        if (shared)
            instance._shared = shared;
    }

    return instance._shared;
}

OPLocalNotificationManager& OPLocalNotificationManager::setType(const std::string& type)
{
    _type = type;
    
    return (*this);
}

void OPLocalNotificationManager::initialize()
{
    setup();
}

void OPLocalNotificationManager::setup()
{
    OPEventBus::shared()->listen<OPEvents::ApplicationWillEnterForeground>(
        [this](const OPEvents::ApplicationWillEnterForeground) {
            closeAllNotifications();
        }
    );
}

void OPLocalNotificationManager::scheduleNotification(const std::string& actionName,
                                                      const std::string& title,
                                                      const std::string& message,
                                                      const int timeRepeat,
                                                      const bool repeat)
{
    int interval = checkTimeRepeat(timeRepeat);
    return setupScheduleNotification(actionName, title, message, -1, -1, interval, repeat);
}

void OPLocalNotificationManager::scheduleNotification(const std::string& actionName,
                                                      const std::string& title,
                                                      const std::string& message,
                                                      const int hours,
                                                      const int minutes,
                                                      const bool repeat)
{
    auto timeInfo = checkTime(hours, minutes);
    return setupScheduleNotification(actionName, title, message,
                                     timeInfo.hour, timeInfo.minute,
                                     0, repeat);
}

void OPLocalNotificationManager::unscheduleNotification(const std::string& actionName)
{
}

void OPLocalNotificationManager::setupScheduleNotification(const std::string& actionName,
                                                           const std::string& title,
                                                           const std::string& message,
                                                           const int hours,
                                                           const int minutes,
                                                           const int timeRepeat,
                                                           const bool repeat)
{
}

OPLocalNotificationManager::TimeInfo OPLocalNotificationManager::checkTime(const int hours, const int minutes) const
{
    int h = hours;
    int m = minutes;

    if (h < 0)
    {
        auto dateTime = OPDateTime::shared();
        dateTime->update();

        h = dateTime->get(OPDateTime::Flag::HOUR);
        m = dateTime->get(OPDateTime::Flag::MINUTE);
    }

    return {h, m};
}

int OPLocalNotificationManager::checkTimeRepeat(const int timeRepeat) const
{
    int interval = timeRepeat;
    if (interval <= 0)
        interval = OP_SECONDS_IN_DAY;

    return interval;
}
