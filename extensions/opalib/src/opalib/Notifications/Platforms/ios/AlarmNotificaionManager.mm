//
//  AlarmNotificaionManager.m
//  opalib-ios
//
//  Created by Opala Studios on 09/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "AlarmNotificaionManager.h"
#import <UserNotifications/UserNotifications.h>

@implementation AlarmNotificaionManager

- (bool) scheduleNotification: (NSString*) identifier
                        title: (NSString*) titleNotification
                         body: (NSString*) message
                          hrs: (int) hours
                          min: (int) minutes
                         time: (int) timeRepeat
                         isRepeat: (bool) repeat
{
    if (@available(iOS 10.0, *))
    {
        UNMutableNotificationContent* content = [self createContent:titleNotification
                                                               body:message];
        
        if (content == nil)
            return false;
        
        UNNotificationTrigger* trigger = nil;
        
        if (hours >= 0 && minutes >= 0)
            trigger = [self createCalendarNotification:hours min:minutes isRepeat:repeat];
        else if (timeRepeat > 0)
            trigger = [self createTimeInterval:timeRepeat isRepeat:repeat];
        
        if (trigger == nil)
            return false;
        
        // Create the request object.
        UNNotificationRequest* request = [UNNotificationRequest
               requestWithIdentifier:identifier content:content trigger:trigger];
        
        return [self registerNotification:request];
    }
    
    return false;
}

- (bool) unscheduleNotification: (NSString*) actionName
{
    if (@available(iOS 10.0, *)) {
        UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
        
        if (center == nil)
            return false;
        
        [center removePendingNotificationRequestsWithIdentifiers:[NSArray arrayWithObjects: actionName, nil]];
    }
    
    return true;
}

- (UNMutableNotificationContent*) createContent:(NSString*) titleNotification
                                           body:(NSString*) message
                                                API_AVAILABLE(ios(10.0))
{
    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
    content.title = [NSString localizedUserNotificationStringForKey:titleNotification arguments:nil];
    content.body = [NSString localizedUserNotificationStringForKey:message arguments:nil];
    content.sound = [UNNotificationSound defaultSound];
    
    return content;
}

- (UNTimeIntervalNotificationTrigger*) createTimeInterval:(int) timeRepeat
                                                 isRepeat: (bool) repeat
                                                          API_AVAILABLE(ios(10.0))
{
    UNTimeIntervalNotificationTrigger* trigger = [UNTimeIntervalNotificationTrigger
                    triggerWithTimeInterval:timeRepeat repeats: repeat? YES : NO];
    
    return trigger;
}

- (UNCalendarNotificationTrigger*) createCalendarNotification:(int) hours
                                                          min:(int) minutes
                                                     isRepeat: (bool) repeat
                                                          API_AVAILABLE(ios(10.0))
{
    NSDateComponents* date = [[NSDateComponents alloc] init];
    date.hour = (NSInteger)hours;
    date.minute = (NSInteger)minutes;
    UNCalendarNotificationTrigger* trigger = [UNCalendarNotificationTrigger
           triggerWithDateMatchingComponents:date repeats: repeat? YES : NO];
    
    return trigger;
}

- (bool) registerNotification:(UNNotificationRequest*) request API_AVAILABLE(ios(10.0))
{
    if (request == nil)
        return false;
        
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    
    if (center == nil)
        return false;
    
    [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
        if (error != nil) {
            NSLog(@"%@", error.localizedDescription);
        }
    }];
        
    return true;
}

@end
