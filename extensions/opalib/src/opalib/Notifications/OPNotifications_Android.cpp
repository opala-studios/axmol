//
//  OPNotifications_Android.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#include <jni.h>
#include <cocos2d/cocos/platform/android/jni/JniHelper.h>
#include "OPNotifications_Android.hpp"

extern "C" {
void Java_com_opalastudios_opalib_activity_OpalibActivity_onPushNotificated(JNIEnv* env, jobject obj, jstring json, jboolean cameFromBackground){
    OPNotificationManager::Notification notification = {};
    notification.payload.Parse(cocos2d::JniHelper::jstring2string(json).c_str());
    notification.cameFromBackground = cameFromBackground;
    OPNotificationManager::shared()->enqueueNotification(std::move(notification));
}
}

OPNotifications_Android::OPNotifications_Android()
{
}

bool OPNotifications_Android::isRegisteredForNotifications()
{
    return false;
}

void OPNotifications_Android::askForPermission()
{
    OPNotificationManager::askForPermission();
}
