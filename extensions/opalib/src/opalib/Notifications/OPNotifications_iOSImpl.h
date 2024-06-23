//
//  OPNotifications_iOSImpl.h
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#import <Foundation/Foundation.h>
#import <FirebaseMessaging/FirebaseMessaging.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPNotifications_iOSImpl : NSObject <FIRMessagingDelegate, UNUserNotificationCenterDelegate>

- (void)initialize;
- (void)applicationDidReceiveRemoteNotification:(NSDictionary *)userInfo;
- (void)applicationDidReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;
- (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
- (void)registerForRemoteNotifications;

@end

NS_ASSUME_NONNULL_END
