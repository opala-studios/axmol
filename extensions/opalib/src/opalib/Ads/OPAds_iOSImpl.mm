//
//  OPAds_iOSImpl.mm
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPAds_iOSImpl.h"
#import "OPBanner.h"
#import <FBAudienceNetwork/FBAudienceNetwork.h>
#import <FirebaseAnalytics/FirebaseAnalytics.h>

@implementation OPAds_iOSImpl

- (void)setup:(OPAdsConfig*)config
{
    _config = config;
    [FBAdSettings setAdvertiserTrackingEnabled:config->userConsent];
    _banner = [[OPBanner alloc] initWithConfig:config];
    _interstitial = [[OPInterstitial alloc] initWithConfig:config];
    _rewarded = [[OPRewarded alloc] initWithConfig:config];
    
    [IronSource setConsent:config->userConsent];
    
#if COCOS2D_DEBUG > 0
    [IronSource setAdaptersDebug:YES];
#endif
    
    [IronSource addImpressionDataDelegate:self];
    
    [IronSource initWithAppKey:[NSString stringWithUTF8String:config->platformConfig.appKey.c_str()]];
    [IronSource shouldTrackReachability:YES];
    [ISIntegrationHelper validateIntegration];
    
    [_banner setup];
    [_interstitial setup];
    [_rewarded setup];
}


- (void)impressionDataDidSucceed:(ISImpressionData*)impressionData {
        [FIRAnalytics logEventWithName:kFIREventAdImpression parameters:@{
            kFIRParameterAdPlatform:@"ironSource",
            kFIRParameterAdSource:impressionData.ad_network,
            kFIRParameterAdFormat:impressionData.ad_unit,
            kFIRParameterAdUnitName:impressionData.instance_name,
            kFIRParameterCurrency:@"USD",
            kFIRParameterValue:impressionData.revenue
        }];
}

- (float)bannerHeight
{
    return _banner.height;
}

- (float)bannerWidth
{
    return _banner.width;
}


- (void)setBannerBackgroundColor:(const cocos2d::Color4B&)color
{
    [_banner setBackgroundColor:color];
}

- (void)showBanner:(OPBannerPosition)position
{
    [_banner show:position];
}

- (void)hideBanner
{
    [_banner hide];
}

- (void)showInterstitial:(const OPAds::SEL_InterstitialHandlerWithCode&)completion
{
    [_interstitial show:completion]; 
}

- (void)showRewarded:(const OPAds::SEL_RewardedHandlerWithCode &)completion
{
    [_rewarded show:completion]; 
}

- (BOOL)isBannerVisible
{
    return [_banner isVisible];
}

- (BOOL)isInterstitialReady
{
    return [_interstitial isReady];
}

- (BOOL)isRewardedReady
{
    return [_rewarded isReady];
}

@end
