//
//  OPRewarded.h
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import <IronSource/IronSource.h>
#import "OPAds.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface OPRewarded : NSObject <ISRewardedVideoDelegate>
{
    long _completionInvokeId;
    BOOL _closed;
    BOOL _containsReward;
    BOOL _showing;
    OPAds::SEL_RewardedHandlerWithCode _completion;
    OPAdsConfig* _config;
}

@property (nonatomic, readonly) BOOL isReady;

- (instancetype)initWithConfig:(OPAdsConfig*)config;

- (void)setup;
- (void)show:(const OPAds::SEL_RewardedHandlerWithCode&)completion;

@end

NS_ASSUME_NONNULL_END
