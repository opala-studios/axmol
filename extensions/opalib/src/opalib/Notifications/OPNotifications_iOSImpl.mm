//
//  OPNotifications_iOSImpl.m
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#import "OPNotifications_iOSImpl.h"

#import "./OPNotificationManager.hpp"
#import "../Debug/OPDebug.hpp"
#import "../UserData/OPUserData.hpp"

#import <FirebaseInstanceID/FirebaseInstanceID.h>
#import <FirebaseMessaging/FirebaseMessaging.h>

#include <Analytics/OPAnalyticsEvent.hpp>

@implementation OPNotifications_iOSImpl

- (void)initialize
{
    UNUserNotificationCenter.currentNotificationCenter.delegate = self;
    [FIRMessaging messaging].delegate = self;
    
    [self fetchToken];
}

- (void)fetchToken
{
    OPDebug::log("Notifications", "Fetching token.");
    
    [[FIRInstanceID instanceID] instanceIDWithHandler:^(FIRInstanceIDResult * _Nullable result, NSError * _Nullable error)
    {
        if (error != nil) {
            OPDebug::log("Notifications", "Error fetching Firebase instance ID: " + std::string(error.localizedDescription.UTF8String));
        }
        else
        {
            std::string token = std::string(result.token.UTF8String);
            
            OPDebug::log("Notifications", "Received instance ID token: " + token);
            OPUserData::shared()->setString("notifications_token", token);
        }
    }];
}

- (void)registerForRemoteNotifications
{
    UIApplication* application = UIApplication.sharedApplication;
    
    if ([UNUserNotificationCenter class] != nil)
    {
        // iOS 10 or later
        // For iOS 10 display notification (sent via APNS)
        [UNUserNotificationCenter currentNotificationCenter].delegate = self;
        UNAuthorizationOptions authOptions = UNAuthorizationOptionAlert |
        UNAuthorizationOptionSound | UNAuthorizationOptionBadge;
        [[UNUserNotificationCenter currentNotificationCenter]
         requestAuthorizationWithOptions:authOptions
         completionHandler:^(BOOL granted, NSError * _Nullable error) {
             // ...
            if (error == nil){
                OPAnalyticsEvent("notification_allowed").set("allowed", granted).send();
            }
         }];
    }
    else
    {
        // iOS 10 notifications aren't available; fall back to iOS 8-9 notifications.
        UIUserNotificationType allNotificationTypes =
        (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
        UIUserNotificationSettings *settings =
        [UIUserNotificationSettings settingsForTypes:allNotificationTypes categories:nil];
        [application registerUserNotificationSettings:settings];
    }
    
    [application registerForRemoteNotifications];
}

- (void)messaging:(FIRMessaging *)messaging didReceiveRegistrationToken:(NSString *)fcmToken
{
    OPDebug::log("Notifications", "Did receive FCM token: " + std::string(fcmToken.UTF8String));
}

- (void)applicationDidReceiveRemoteNotification:(NSDictionary *)userInfo
{
    OPDebug::log("Notifications", "Did receive remote notification.");
}

- (rapidjson::Document)convertDictionaryPayload:(NSDictionary*)dictionary
{
    rapidjson::Document result;
    result.SetObject();
    
    auto& allocator = result.GetAllocator();
    
    for (NSString* key in dictionary)
    {
        if ([key hasPrefix:@"google."] || [key hasPrefix:@"gcm."] || [key isEqualToString:@"aps"]) {
            continue;
        }
        
        id value = dictionary[key];
        
        if ([value isKindOfClass:NSString.class]) {
            NSString* valueStr = (NSString*)value;
            rapidjson::Value value;
            value.SetString(valueStr.UTF8String, valueStr.length, allocator);
            
            rapidjson::Value keyStr;
            keyStr.SetString(key.UTF8String, key.length, allocator);
            result.AddMember(keyStr.Move(), value.Move(), allocator);
        }
    }
    
    return result;
}

- (void)applicationDidReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler
{
    OPDebug::log("Notifications", "Did receive remote notification with completion handler.");
    
    [[FIRMessaging messaging] appDidReceiveMessage:userInfo];
    
    completionHandler(UIBackgroundFetchResultNewData);
}

- (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [FIRMessaging messaging].APNSToken = deviceToken;
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler
{
    [[FIRMessaging messaging] appDidReceiveMessage:notification.request.content.userInfo];
    
    NSDictionary* userInfo = notification.request.content.userInfo;
    
    OPNotificationManager::Notification notif = {};
    notif.payload = [self convertDictionaryPayload:userInfo];
    notif.cameFromBackground = false;
    
    OPNotificationManager::shared()->enqueueNotification(std::move(notif));
    
    completionHandler(UNNotificationPresentationOptionNone);
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)(void))completionHandler
{
    [[FIRMessaging messaging] appDidReceiveMessage:response.notification.request.content.userInfo];
    
    NSDictionary* userInfo = response.notification.request.content.userInfo;
    OPNotificationManager::Notification notif = {};
    notif.payload = [self convertDictionaryPayload:userInfo];
    notif.cameFromBackground = true;
    
    OPNotificationManager::shared()->enqueueNotification(std::move(notif));
    
    completionHandler();
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center openSettingsForNotification:(nullable UNNotification *)notification
{
    OPDebug::log("Notifications", "Open settings for notification.");
}

@end
