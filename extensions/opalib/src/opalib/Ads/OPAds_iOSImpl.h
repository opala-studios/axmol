//
//  OPAds_iOSImpl.h
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPAds.hpp"
#import "OPAdsConfig.hpp"
#import "OPBanner.h"
#import "OPInterstitial.h"
#import "OPRewarded.h"
#import <IronSource/IronSource.h>

NS_ASSUME_NONNULL_BEGIN

@interface OPAds_iOSImpl : NSObject <ISImpressionDataDelegate>
{
    OPAdsConfig* _config;
    
    OPBanner *_banner;
    OPInterstitial* _interstitial;
    OPRewarded* _rewarded;
}

- (void)setup:(OPAdsConfig*)config;

- (void)showBanner:(OPBannerPosition)position;
- (void)hideBanner;

- (void)showInterstitial:(const OPAds::SEL_InterstitialHandlerWithCode&)completion;
- (void)showRewarded:(const OPAds::SEL_RewardedHandlerWithCode&)completion;

- (BOOL)isBannerVisible;
- (BOOL)isInterstitialReady;
- (BOOL)isRewardedReady;

- (float)bannerHeight;
- (float)bannerWidth;

- (void)setBannerBackgroundColor:(const cocos2d::Color4B&)color;

@end

NS_ASSUME_NONNULL_END
