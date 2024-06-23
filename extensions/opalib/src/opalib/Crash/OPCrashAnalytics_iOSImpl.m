//
//  OPCrashAnalytics_iOSImpl.m
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 04/11/19.
//

#import "OPCrashAnalytics_iOSImpl.h"
@import FirebaseCrashlytics;

@implementation OPCrashAnalytics_iOSImpl

- (void)setup
{
    // Crashlytics is automatically initialized with Firebase
}

- (void)verbose:(NSString*)message
{
    #if DEBUG
        NSLog(@"OPALIB | Crashlytics | VERBOSE: %@", message);
    #endif
    [[FIRCrashlytics crashlytics] logWithFormat:@"OPALIB | Crashlytics | VERBOSE: %@", message];
}

- (void)log:(NSString*)message
{
    #if DEBUG
        NSLog(@"OPALIB | Crashlytics | LOG: %@", message);
    #endif
    [[FIRCrashlytics crashlytics] logWithFormat:@"OPALIB | Crashlytics | LOG: %@", message];
}

- (void)warning:(NSString*)message
{
    #if DEBUG
        NSLog(@"OPALIB | Crashlytics | WARNING: %@", message);
    #endif
    [[FIRCrashlytics crashlytics] logWithFormat:@"OPALIB | Crashlytics | WARNING: %@", message];
}

- (void)error:(NSString*)message
{
    #if DEBUG
        NSLog(@"OPALIB | Crashlytics | ERROR: %@", message);
    #endif
    [[FIRCrashlytics crashlytics] logWithFormat:@"OPALIB | Crashlytics | ERROR: %@", message];
}

- (void)recordException:(NSString*)message
{
    #if DEBUG
        NSLog(@"OPALIB | Crashlytics | RECORD EXCEPTION: %@", message);
    #endif
    NSDictionary *userInfo = @{
        NSLocalizedDescriptionKey: message,
    };

    NSError *error = [NSError errorWithDomain:NSCocoaErrorDomain
                      code:-1001
                      userInfo:userInfo];

    [[FIRCrashlytics crashlytics] recordError:error];
}

@end
