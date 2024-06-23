//
//  OPCrashAnalytics_iOSImpl.h
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 04/11/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPCrashAnalytics_iOSImpl : NSObject

- (void)setup;

- (void)verbose:(NSString*)message;
- (void)log:(NSString*)message;
- (void)warning:(NSString*)message;
- (void)error:(NSString*)message;
- (void)recordException:(NSString*)message;

@end

NS_ASSUME_NONNULL_END
