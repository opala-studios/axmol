//
//  OPAppController.m
//  opalib-ios
//
//  Created by Opala Teste on 03/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#import "OPAppController.h"
//#include <AdjustSdk/Adjust.h>
#include <cocos2d/external/json/document.h>
#include <Notifications/OPNotificationManager.hpp>

@implementation OPAppController


- (BOOL) application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler{
    if ([[userActivity activityType] isEqualToString:NSUserActivityTypeBrowsingWeb]) {
        NSURL* url = [userActivity webpageURL];
        
//        [Adjust appWillOpenUrl:url];
//        
//        NSURL *oldStyleDeeplink = [Adjust convertUniversalLink:url scheme:@"opala-superpads"];
//
//        [self application:application openURL:oldStyleDeeplink options:@{}];
    }
    
    return true;
}

- (BOOL) application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    
    OPNotificationManager::Notification notification = {};
    notification.cameFromBackground = true;
    notification.payload.SetObject();
    
    auto& allocator = notification.payload.GetAllocator();
    
    rapidjson::Value scheme;
    scheme.SetString(url.scheme.UTF8String, url.scheme.length, allocator);
    
    notification.payload.AddMember("scheme", scheme.Move(), allocator);
    
    NSURLComponents* components = [NSURLComponents componentsWithURL:url resolvingAgainstBaseURL:true];
    if (components != nil){
        for (NSURLQueryItem* item in components.queryItems){
            
            rapidjson::Value key;
            key.SetString(item.name.UTF8String, item.name.length, allocator);
            
            rapidjson::Value value;
            value.SetString(item.value.UTF8String, item.value.length, allocator);
            notification.payload.AddMember(key.Move(), value.Move(), allocator);
        }
    }
    
    OPNotificationManager::shared()->enqueueNotification(std::move(notification));
    return true;
}


@end
