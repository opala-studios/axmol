//
//  OPInterstitial.h
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import "OPAdsConfig.hpp"
#import <UIKit/UIKit.h>
#import <IronSource/IronSource.h>
#import "OPAds.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface OPInterstitial : NSObject <ISInterstitialDelegate>
{
    OPAds::SEL_InterstitialHandlerWithCode _completion;
    NSDate* _lastShowDate;
    OPAdsConfig* _config;
    BOOL _showing;
}

@property (nonatomic, readonly) BOOL isReady;
@property (nonatomic, readonly) BOOL isIntervalOK;

- (instancetype)initWithConfig:(OPAdsConfig*)config;

- (void)setup;
- (void)show:(const OPAds::SEL_InterstitialHandlerWithCode&)completion;

@end

NS_ASSUME_NONNULL_END
