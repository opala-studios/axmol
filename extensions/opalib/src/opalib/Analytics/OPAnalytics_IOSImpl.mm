//
//  OPAnalyticsIOSImpl.m
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#import "OPAnalytics_IOSImpl.h"
#import "Opalib.h"

#include <iostream>

#import <FirebaseCore/FirebaseCore.h>
#import <FirebaseAnalytics/FirebaseAnalytics.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>

@implementation OPAnalytics_IOSImpl

- (void)setupFirebase
{
    OPDebug::log("Analytics", "Initializing Firebase");
}

- (void)setupFacebook
{
    [FBSDKApplicationDelegate initializeSDK:nil];
}

- (void)setup
{
    [self setupFirebase];
    [self setupFacebook];
}

- (NSDictionary<NSString*, id>*)convertParameters:(const OPAnalyticsEvent::ParametersMap&)parameters
{
    NSMutableDictionary<NSString*, id>* dictionary = [[NSMutableDictionary<NSString*, id> alloc] init];
    
    for (auto param : parameters)
    {
        NSString* key = [NSString stringWithUTF8String:param.first.c_str()];
        
        switch (param.second.getType())
        {
            case OPParameter::Type::BOOL:
                dictionary[key] = [[NSNumber alloc] initWithBool:param.second.getBool()];
                break;
                
            case OPParameter::Type::INT:
                dictionary[key] = [[NSNumber alloc] initWithInt:param.second.getInt()];
                break;
                
            case OPParameter::Type::FLOAT:
                dictionary[key] = [[NSNumber alloc] initWithFloat:param.second.getFloat()];
                break;
                
            case OPParameter::Type::DOUBLE:
                dictionary[key] = [[NSNumber alloc] initWithDouble:param.second.getDouble()];
                break;
                
            case OPParameter::Type::STRING:
                dictionary[key] = [NSString stringWithUTF8String:param.second.getCString()];
                break;
                
            default:
                break;
        }
    }
    
    return dictionary;
}

- (void)sendToFacebookWithName:(NSString*)name parameters:(NSDictionary<NSString*, id>*)params
{
    [FBSDKAppEvents logEvent:name parameters:params];
}

- (void)sendToFirebaseWithName:(NSString*)name parameters:(NSDictionary<NSString*, id>*)params
{
    if ([name isEqual:@"screen_view"]) {
        [FIRAnalytics setScreenName:params[@"screen_name"] screenClass:nil];
    }
    else {
        [FIRAnalytics logEventWithName:name parameters:params];
    }
}

- (void)sendEvent:(const OPAnalyticsEvent&)event
{
    NSString* name = [NSString stringWithUTF8String:event.getName().c_str()];
    NSDictionary<NSString*, id>* params = [self convertParameters:event.getParameters()];
    
    [self sendToFirebaseWithName:name parameters:params];
    
    [params release];
}

- (void)setUserProperty:(const std::string&)key withValue:(const std::string&)value
{
    NSString* nsKey = [NSString stringWithUTF8String:key.c_str()];
    NSString* nsValue = [NSString stringWithUTF8String:value.c_str()];
    [FIRAnalytics setUserPropertyString:nsValue forName:nsKey];
}

@end
