//
//  OPAds_Android.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 04/07/19.
//

#ifndef OPAds_Android_hpp
#define OPAds_Android_hpp

#include "./OPAds.hpp"
#include "platform/android/jni/JniHelper.h"

class OPAds_Android : public OPAds
{
protected:

    virtual void setup(OPAdsConfig* config) override;//, bool hasConsent = true) override;

    OPAds::SEL_InterstitialHandlerWithCode _interstitialCompletion {0};
    OPAds::SEL_RewardedHandlerWithCode _rewardedCompletion {0};

    virtual void _showBanner(OPBannerPosition position) override;
    virtual void _hideBanner() override;

    virtual void _showInterstitial(const SEL_InterstitialHandlerWithCode& handler) override;
    virtual void _showRewarded(const SEL_RewardedHandlerWithCode& handler) override;

    virtual bool _isBannerVisible() override;
    virtual bool _isInterstitialReady() override;
    virtual bool _isRewardedReady() override;

    virtual float _getBannerHeight() override;
    virtual float _getBannerWidth() override;

    void _setBannerBackgroundColor(cocos2d::Color4B color) override;
    float _getScreenHeightFactor() override;

public:
    
    OPAds_Android();

    void onInterstitialDone(int code);
    void onRewardedDone(int code);
};

#endif /* OPAds_Android_hpp */
