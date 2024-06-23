//
//  OPRemoteConfig_iOSImpl.m
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 01/11/19.
//

#import "OPRemoteConfig_iOSImpl.h"
#import <FirebaseRemoteConfig/FirebaseRemoteConfig.h>

@implementation OPRemoteConfig_iOSImpl

- (void)setupWithDefaultsPath:(NSString*)path
{
    FIRRemoteConfigSettings *settings = [FIRRemoteConfigSettings new];
    NSTimeInterval time;

#if COCOS2D_DEBUG >= 1
    time = 10;
#else
    time = 3600;
#endif
    
    [settings setMinimumFetchInterval:time];
    [[FIRRemoteConfig remoteConfig] setConfigSettings:settings];
    
    NSDictionary<NSString*, NSObject*>* configDefaults = [self loadDefaultsFromFile:path];
    if (configDefaults) {
        [[FIRRemoteConfig remoteConfig] setDefaults:configDefaults];
    }
}

- (void)fetchAndActivate:(void(^)(bool))completion
{
    [[FIRRemoteConfig remoteConfig] fetchAndActivateWithCompletionHandler:^(FIRRemoteConfigFetchAndActivateStatus status, NSError * _Nullable error)
    {
        BOOL success = (status == FIRRemoteConfigFetchAndActivateStatusSuccessFetchedFromRemote ||
                        status == FIRRemoteConfigFetchAndActivateStatusSuccessUsingPreFetchedData);

        if (completion) {
            completion(success);
        }
    }];
}

- (NSDictionary<NSString*, NSObject*>*)loadDefaultsFromFile:(NSString*)filePath
{
    NSData* data = [[NSData alloc] initWithContentsOfFile:filePath];
    if (!data) {
        return 0;
    }
    
    NSError* error = 0;
    NSDictionary* root = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
    [data release];
    
    if (error) {
        return 0;
    }
    
    return root;
}

- (BOOL)getBool:(NSString*)key
{
    return [[[FIRRemoteConfig remoteConfig] configValueForKey:key] boolValue];
}

- (long)getLong:(NSString*)key
{
    return [[[FIRRemoteConfig remoteConfig] configValueForKey:key] numberValue].longValue;
}

- (double)getDouble:(NSString*)key
{
    return [[[FIRRemoteConfig remoteConfig] configValueForKey:key] numberValue].doubleValue;
}

- (NSString*)getString:(NSString*)key
{
    return [[[FIRRemoteConfig remoteConfig] configValueForKey:key] stringValue];
}

@end
