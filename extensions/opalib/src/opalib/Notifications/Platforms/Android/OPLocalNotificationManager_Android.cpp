//
// Created by Opala Studios on 03/09/21.
//

#include "OPLocalNotificationManager_Android.hpp"
#include "../../../Utils/OPDateTime.hpp"
#include <opalib/Opalib.h>

OPLocalNotificationManager_Android::OPLocalNotificationManager_Android() :
                                    OPLocalNotificationManager(),
                                    _className("com/opalastudios/opalib/notifications/AlarmNotificationManager")
{}

OPLocalNotificationManager_Android::~OPLocalNotificationManager_Android()
{}

void OPLocalNotificationManager_Android::setup()
{
    OPLocalNotificationManager::setup();

    std::string method = "setup";
    cocos2d::JniHelper::callStaticVoidMethod(_className, method);
}

void OPLocalNotificationManager_Android::unscheduleNotification(const std::string& actionName)
{
    std::string method = "unscheduleNotification";

    bool res = cocos2d::JniHelper::callStaticBooleanMethod(_className, method, actionName);

    if (!res)
        OP_LOG("Falha ao remover o agendamento da notificação. Verifique os parâmetros, "
               "se estiverem corretos contate o desenvolvedor.");
}

void OPLocalNotificationManager_Android::setupScheduleNotification(const std::string& actionName,
                                                                   const std::string& title,
                                                                   const std::string& message,
                                                                   const int hours,
                                                                   const int minutes,
                                                                   const int timeRepeat,
                                                                   const bool repeat)
{
    std::string method = "scheduleNotification";
    int milliseconds = timeRepeat * 1000;

    bool res = cocos2d::JniHelper::callStaticBooleanMethod(_className, method,
                                                           _type, actionName,
                                                           title, message,
                                                           hours, minutes,
                                                           milliseconds, repeat);
    if (!res)
        OP_LOG("Falha ao agendar a notificação. Verifique os parâmetros, "
               "se estiverem corretos contate o desenvolvedor.");
}

void OPLocalNotificationManager_Android::closeAllNotifications()
{
    std::string method = "closeAllNotifications";
    cocos2d::JniHelper::callStaticVoidMethod(_className, method);
}
