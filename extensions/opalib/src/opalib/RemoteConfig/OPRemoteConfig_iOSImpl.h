//
//  OPRemoteConfig_iOSImpl.h
//  InfiniteCocos-mobile
//
//  Created by Adilson Tavares on 01/11/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPRemoteConfig_iOSImpl : NSObject

- (void)setupWithDefaultsPath:(NSString*)path;
- (void)fetchAndActivate:(void(^)(bool))completion;

- (BOOL)getBool:(NSString*)key;
- (long)getLong:(NSString*)key;
- (double)getDouble:(NSString*)key;
- (NSString*)getString:(NSString*)key;

@end

NS_ASSUME_NONNULL_END
