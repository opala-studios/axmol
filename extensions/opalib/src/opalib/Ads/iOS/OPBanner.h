//
//  OPBanner.h
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#import <IronSource/IronSource.h>
#import "OPAdsConfig.hpp"
#import "OPBannerPosition.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface OPBanner : NSObject <ISBannerDelegate>
{
    UIView* _background;
    BOOL _needsConstraintsUpdate;
    OPBannerPosition _bannerPosition;
    OPAdsConfig* _config;
    ISBannerView *_bannerView;
}

@property (nonatomic, readonly) BOOL wantsBanner;
@property (nonatomic, readonly) BOOL isLoading;
@property (nonatomic, readonly) BOOL isReady;
@property (nonatomic, readonly) BOOL isVisible;

- (instancetype)initWithConfig:(OPAdsConfig*)config;

- (void)setBackgroundColor:(const cocos2d::Color4B&)color;

- (void)setup;
- (void)show:(OPBannerPosition)position;
- (void)hide;
- (float)height;
- (float)width;

@end

NS_ASSUME_NONNULL_END
