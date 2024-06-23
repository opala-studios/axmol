//
//  OPAds.hpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 19/06/19.
//

#ifndef OPAds_hpp
#define OPAds_hpp

#include <cocos2d.h>
#include <iostream>
#include <map>

#include "./OPAdsConfig.hpp"
#include "./OPBannerPosition.hpp"
#include "../Analytics/OPAnalyticsEvent.hpp"
#include "../UserData/OPUserData.hpp"
#include "../Datetime/OPTimer.hpp"

enum class OPAdsResult
{
    UNINITIALIZED, // -2000
    FAILED_UNKNOWN, // -1000
    DISABLED,
    TEMPORARILY_DISABLED,
    SHOWED_BUT_FAILED, // -100
    SHOWED_BUT_INCOMPLETE, // -2
    SUCCESS, // -1
    SHOWED, // 0
    IS_ALREADY_SHOWING_AN_AD, // 1022 (Rewarded) e 1036 (Interstitial)
    IS_NOT_AD_INTERVAL_OK,
    INIT_FAILED, // 508
    NO_ADS_TO_SHOW, // 509 (Interstitial) e 1023 (Rewarded)
    FAILED_NO_FILL_RATE, // 510
    FAILED_NO_CONNECTION, // 520
};

class OPAds
{
public:
    struct InterstitialLoaded {};
    
    typedef std::function<void(const int)> SEL_InterstitialHandlerWithCode;
    typedef std::function<void(const int)> SEL_RewardedHandlerWithCode;
    typedef std::function<void(const OPAdsResult)> SEL_InterstitialHandler;
    typedef std::function<void(const OPAdsResult)> SEL_RewardedHandler;

protected:

    bool initialized {false};

    OPAdsConfig config;
    OPBannerPosition bannerPosition;
    bool wantsBanner {false};

    bool bannerEnabled {true};
    bool interstitialEnabled {true};
    bool showingInterstitial {false};
    bool showingRewarded {false};

    std::map<std::string, int> interstitialTriggers;

    static std::string configFilePath;
    
    virtual void setup(OPAdsConfig* config) = 0;
    static void debugLog(const std::string& text);
    
    OPAds();

    virtual void _showBanner(OPBannerPosition position) = 0;
    virtual void _hideBanner() = 0;
    virtual void _showInterstitial(const SEL_InterstitialHandlerWithCode& handler) = 0;
    virtual void _showRewarded(const SEL_RewardedHandlerWithCode& handler) = 0;

    virtual bool _isBannerVisible() = 0;
    virtual bool _isInterstitialReady() = 0;
    virtual bool _isRewardedReady() = 0;
    virtual void _setBannerBackgroundColor(cocos2d::Color4B color) = 0;
    virtual float _getScreenHeightFactor() = 0;
    virtual float _getBannerHeight() = 0;
    virtual float _getBannerWidth() = 0;
    
    int getTotalInterstitialCount() { return OPUserData::shared()->getInt("interstitial_count"); }
    void setTotalInterstitialCount(int total) { OPUserData::shared()->setInt("interstitial_count", total); }

    void increaseInterstitialCount();
    
    void initAdsResultMap();
    
    OPAdsResult getResult(const int value);

    int minInterstitialInterval {0};
    OPTimer interstitialTimer;
    std::map<int, OPAdsResult> adsResultMap;
    
public:
    
    void initialize(bool hasConsent = true);
    bool isInitialized();
    
    static OPAds *shared();
    
    void showBanner();
    void showBanner(OPBannerPosition position);
    void hideBanner();
    void dispatchBannerVisibilityChangeEvent();


    virtual void setMinInterstitialInterval(int seconds) 
    {
        minInterstitialInterval = seconds;
        OPDebug::log("OPAds", "Set min interstitial interval: " + std::to_string(seconds));
    }

    void setBannerEnabled(bool enabled)
    {
        bannerEnabled = enabled;

        if (!enabled) {
            hideBanner();
        }
    }

    void setInterstitialEnabled(bool enabled) {
        interstitialEnabled = enabled;
    }
    
    void showInterstitial(const SEL_InterstitialHandler& handler, bool ignoresMinInterval = false);
    void showInterstitial(const OPAnalyticsEvent& event, const SEL_InterstitialHandler& handler, bool ignoresMinInterval = false);

    void showInterstitialIfNeeded(const std::string& trigger, const SEL_InterstitialHandler& handler, bool ignoresMinInterval = false);
    bool isInterstitialIntervalOK();

    void showRewarded(const SEL_RewardedHandler& handler);
    void showRewarded(const OPAnalyticsEvent& event, const SEL_RewardedHandler& handler);

    virtual bool isBannerWanted();
    bool isBannerVisible();
    bool isInterstitialReady();
    bool isRewardedReady();

    void setBannerBackgroundColor(cocos2d::Color4B color);
    void setBannerBackgroundPadding(int left, int top,int right,int bottom);
    float getScreenHeightFactor();

    float getBannerHeight();
    float getBannerWidth();
    
    int getInterstitialCount(const std::string& trigger);
    
    void onInterstitialLoaded();    
};

#endif /* OPAds_hpp */
