//
//  OPAds_iOS.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#include "OPAds_iOS.hpp"
#include "OPAds_iOSImpl.h"
#import <UIKit/UIKit.h>
#import <GoogleMobileAds/GoogleMobileAds.h>

void OPAds_iOS::setup(OPAdsConfig* config)
{
    [GADMobileAds sharedInstance].audioVideoManager.audioSessionIsApplicationManaged = YES;
    impl = [[OPAds_iOSImpl alloc] init];
    [((OPAds_iOSImpl*)impl) setup:config];
}

void OPAds_iOS::_showBanner(OPBannerPosition position)
{
    [((OPAds_iOSImpl*)impl) showBanner:position];
}

void OPAds_iOS::_hideBanner()
{
    [((OPAds_iOSImpl*)impl) hideBanner];
}

float OPAds_iOS::_getBannerHeight()
{
    float bannerHeight = [(OPAds_iOSImpl*)impl bannerHeight];
    float windowSize = [UIScreen mainScreen].bounds.size.height;
    float visibleHeight = cocos2d::Director::getInstance()->getVisibleSize().height;
    
    return (bannerHeight / windowSize) * visibleHeight;
}

float OPAds_iOS::_getBannerWidth() {
    float bannerWidth = [(OPAds_iOSImpl*)impl bannerWidth];
    float windowSize = [UIScreen mainScreen].bounds.size.width;
    float visibleWidth = cocos2d::Director::getInstance()->getVisibleSize().width;

    return (bannerWidth / windowSize) * visibleWidth;
}

void OPAds_iOS::_showInterstitial(const SEL_InterstitialHandlerWithCode& handler)
{
    [((OPAds_iOSImpl*)impl) showInterstitial:handler];
}

void OPAds_iOS::_showRewarded(const SEL_RewardedHandlerWithCode& handler)
{
    [((OPAds_iOSImpl*)impl) showRewarded:handler];
}

bool OPAds_iOS::_isBannerVisible()
{
    return [((OPAds_iOSImpl*)impl) isBannerVisible];
}

bool OPAds_iOS::_isInterstitialReady()
{
    return [((OPAds_iOSImpl*)impl) isInterstitialReady];
}

bool OPAds_iOS::_isRewardedReady()
{
    return [((OPAds_iOSImpl*)impl) isRewardedReady];
}

void OPAds_iOS::_setBannerBackgroundColor(cocos2d::Color4B color)
{
    [((OPAds_iOSImpl*)impl) setBannerBackgroundColor:color];
}

float OPAds_iOS::_getScreenHeightFactor(){
    
    return 0;
}
