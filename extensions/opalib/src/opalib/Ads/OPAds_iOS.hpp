//
//  OPAds_iOS.hpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#ifndef OPAds_iOS_hpp
#define OPAds_iOS_hpp

#include "OPAds.hpp"

class OPAds_iOS : public OPAds
{
protected:
    
    void* impl;
    virtual void setup(OPAdsConfig* config) override;
    
    virtual void _showBanner(OPBannerPosition position) override;
    virtual void _hideBanner() override;
    
    virtual void _showInterstitial(const SEL_InterstitialHandlerWithCode& handler) override;
    virtual void _showRewarded(const SEL_RewardedHandlerWithCode& handler) override;

    virtual bool _isBannerVisible() override;
    virtual bool _isInterstitialReady() override;
    virtual bool _isRewardedReady() override;

    virtual float _getBannerHeight() override;
    virtual float _getBannerWidth() override;

    virtual void _setBannerBackgroundColor(cocos2d::Color4B color) override;
    virtual float _getScreenHeightFactor() override;

public:
    
    OPAds_iOS() : OPAds() { }
};

#endif /* OPAds_iOS_hpp */

