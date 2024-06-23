//
// Created by Opala Studios on 03/09/21.
//

#ifndef PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_ANDROID_HPP
#define PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_ANDROID_HPP

#include "opalib/Notifications/OPLocalNotificationManager.hpp"

class OPLocalNotificationManager_Android : public OPLocalNotificationManager
{
public:
    OPLocalNotificationManager_Android();
    ~OPLocalNotificationManager_Android();

    void setup() override;

    void unscheduleNotification(const std::string& actionName) override;

    void closeAllNotifications() override;

protected:

    void setupScheduleNotification(const std::string& actionName, const std::string& title,
                                   const std::string& message, const int hours,
                                   const int minutes, const int timeRepeat,
                                   const bool repeat) override;

private:
    std::string _className;
};

#endif //PROJ_ANDROID_OPLOCALNOTIFICATIONMANAGER_ANDROID_HPP
