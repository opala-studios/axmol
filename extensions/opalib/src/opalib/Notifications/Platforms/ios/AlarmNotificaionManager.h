//
//  AlarmNotificaionManager.hpp
//  opalib-ios
//
//  Created by Opala Studios on 09/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>

NS_ASSUME_NONNULL_BEGIN

@interface AlarmNotificaionManager : NSObject;

- (bool) scheduleNotification: (NSString*) identifier
                        title: (NSString*) titleNotification
                         body: (NSString*) message
                          hrs: (int) hours
                          min: (int) minutes
                         time: (int) timeRepeat
                       isRepeat: (bool) repeat;

- (bool) unscheduleNotification: (NSString*) actionName;

- (UNMutableNotificationContent*) createContent:(NSString*) titleNotification
                                           body:(NSString*) message
                                           API_AVAILABLE(ios(10.0));

- (UNTimeIntervalNotificationTrigger*) createTimeInterval:(int) timeRepeat
                                                 isRepeat:(bool) repeat
                                                 API_AVAILABLE(ios(10.0));

- (UNCalendarNotificationTrigger*) createCalendarNotification:(int) hours
                                                          min:(int) minutes
                                                     isRepeat:(bool) repeat
                                                     API_AVAILABLE(ios(10.0));

- (bool) registerNotification:(UNNotificationRequest*) request API_AVAILABLE(ios(10.0));

@end

NS_ASSUME_NONNULL_END
