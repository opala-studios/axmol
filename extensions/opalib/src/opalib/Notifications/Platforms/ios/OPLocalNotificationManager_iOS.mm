//
//  OPLocalNotificationManager_iOS.cpp
//  opalib-ios
//
//  Created by Opala Studios on 09/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPLocalNotificationManager_iOS.hpp"
#import <UIKit/UIKit.h>
#include "AlarmNotificaionManager.h"

OPLocalNotificationManager_iOS::OPLocalNotificationManager_iOS() :
                                _imp(nullptr)
{}

OPLocalNotificationManager_iOS::~OPLocalNotificationManager_iOS()
{
    _imp = nullptr;
}

void OPLocalNotificationManager_iOS::setup()
{
    OPLocalNotificationManager::setup();
    
    _imp = [[AlarmNotificaionManager alloc] init];
}

void OPLocalNotificationManager_iOS::setupScheduleNotification(const std::string& actionName,
                                                               const std::string& title,
                                                               const std::string& message,
                                                               const int hours,
                                                               const int minutes,
                                                               const int timeRepeat,
                                                               const bool repeat)
{
    NSString* key = [NSString stringWithUTF8String:actionName.c_str()];
    NSString* titleNotification = [NSString stringWithUTF8String:title.c_str()];
    NSString* body = [NSString stringWithUTF8String:message.c_str()];
    
    [((AlarmNotificaionManager*)_imp) scheduleNotification:key title:titleNotification body:body hrs:hours min:minutes time:timeRepeat isRepeat:repeat];
}

void OPLocalNotificationManager_iOS::unscheduleNotification(const std::string& actionName)
{
    NSString* key = [NSString stringWithCString:actionName.c_str()
                                       encoding:[NSString defaultCStringEncoding]];
    [((AlarmNotificaionManager*)_imp) unscheduleNotification:key];
}

