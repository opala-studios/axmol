
//
//  OPAds.cpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 19/06/19.
//

#include "../Application/OPApplication.hpp"
#include "./OPAds.hpp"
#include "../Helpers/OPString.hpp"
#include "../Json/OPJson.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "../Audio/OPAudioManager.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Analytics/OPAnalyticsEvent.hpp"
#include "../Crash/OPCrashAnalytics.hpp"
#include "../Loading/OPLoadingView.hpp"
#include "../RemoteConfig/OPRemoteConfig.hpp"
#include "../Events/OPEvents.hpp"
#include "../Misc/OPEventBus.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPAds_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPAds_Android.hpp"
#include "OPAds.hpp"

#endif

#include <json/rapidjson.h>
#include <json/document.h>

USING_NS_CC;

std::string OPAds::configFilePath = "config/ads.json";

OPAds::OPAds()
{
    initAdsResultMap();
}

OPAds *OPAds::shared()
{
    static OPAds *instance = 0;
    
    if (!instance)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        instance = new OPAds_iOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        instance = new OPAds_Android();
#endif
    }
    
    return instance;
}

void OPAds::initialize(bool hasConsent)
{
    if (initialized){
        return;
    }
    OPDebug::log("Ads", "Initialize");

    rapidjson::Document document;
    OPDebug::ensure(OPJson::parseFile(configFilePath, document), "Could not load config/ads.json");
    
    OPDebug::log("OPAds", "Creating configuration based on JSON");
    config = OPAdsConfig(document);
    //Adding userConsent
    config.userConsent = hasConsent;
    bannerPosition = config.bannerPosition;
    setup(&config);
    
    initialized = true;
}

bool OPAds::isInterstitialIntervalOK()
{
    if (!initialized)
        return false;
    double elapsedTime = interstitialTimer.getElapsedTime();
    return !interstitialTimer.isRunning() || elapsedTime >= minInterstitialInterval;
}

void OPAds::showInterstitial(const SEL_InterstitialHandler& handler, bool ignoresMinInterval)
{
    assert(handler);

    if (!initialized)
    {
        handler(OPAdsResult::UNINITIALIZED);
        return;
    }

    OPCrashAnalytics::shared()->log("OPAds", "Show interstitial.");

    if (!interstitialEnabled) 
    {
        OPCrashAnalytics::shared()->log("OPAds", "Not showing interstitial because interstitial ads are disabled.");
        
        handler(OPAdsResult::DISABLED);
        return;
    }

    if (showingInterstitial || showingRewarded) 
    {
        OPCrashAnalytics::shared()->log("OPAds", "Not showing interstitial because another ad is already being showed.");
        
        handler(OPAdsResult::IS_ALREADY_SHOWING_AN_AD);
        return;
    }

    if (!isInterstitialIntervalOK())
    {
        OPCrashAnalytics::shared()->log("OPAds", "Not showing interstitial because of minimum interstitial interval (current=" + std::to_string(interstitialTimer.getElapsedTime()) + ", min=" + std::to_string(minInterstitialInterval) + ")");
        
        handler(OPAdsResult::IS_NOT_AD_INTERVAL_OK);
        return;
    }

    OPCrashAnalytics::shared()->log("OPAds", "Interstitial interval is OK. Last one was shown " + std::to_string(interstitialTimer.getElapsedTime()) + "s ago (min=" + std::to_string(minInterstitialInterval) + ")");

    showingInterstitial = true;
    
    _showInterstitial([this, handler](const int code) {
        auto result = getResult(code);
        if (result == OPAdsResult::SHOWED){
            OPCocosThread::invoke("interstitial_call_handler", [this, handler, result]()
            {
                handler(result);
            });
            return;
        }
        OPLoadingView::shared()->hide();
        OPApplication::shared()->getAudioEngine()->resume();

        
        if (handler) 
        {
            OPCocosThread::invoke("interstitial_call_handler", [this, handler, result]()
            {
                handler(result);
            });
        }
        
        showingInterstitial = false;

        if (result != OPAdsResult::SHOWED) {
            OPCrashAnalytics::shared()->warn("OPAds", "Interstitial was NOT shown. Probably it is not loaded yet. Code: " + std::to_string(code));
        }
        else {
            OPCrashAnalytics::shared()->log("OPAds", "Interstitial completion was called.");
            interstitialTimer.restart();
        }
    });

    OPCrashAnalytics::shared()->log("OPAds", "Interstitial was shown. Waiting for completion.");
    OPApplication::shared()->getAudioEngine()->pause();
    OPLoadingView::shared()->show();
    
//    if (shown)
//    {
//        OPCrashAnalytics::shared()->log("OPAds", "Interstitial was shown. Waiting for completion.");
//        OPApplication::shared()->getAudioEngine()->pause();
//        OPLoadingView::shared()->show();
//        
//        return OPAdsResult::SHOWED;
//    }
//    else 
//    {
//        OPCrashAnalytics::shared()->warn("OPAds", "Interstitial was NOT shown. Probably it is not loaded yet.");
//        showingInterstitial = false;
//    }
}

void OPAds::showInterstitial(const OPAnalyticsEvent& event, const SEL_InterstitialHandler& handler, bool ignoresMinInterval)
{
    assert(handler);
    
    if (!initialized)
    {
        handler(OPAdsResult::UNINITIALIZED);
        return;
    }

    showInterstitial([handler, event](const OPAdsResult result) {
        if (handler)
            handler(result);
        
        if (result == OPAdsResult::SHOWED)
            event.send();
    }, ignoresMinInterval);
}

void OPAds::showRewarded(const OPAnalyticsEvent& event, const SEL_RewardedHandler& handler)
{
    assert(handler);
    
    if (!initialized)
    {
        handler(OPAdsResult::UNINITIALIZED);
        return;
    }

    showRewarded([handler, event](const OPAdsResult result) {
        if (handler)
            handler(result);
        
        if (result == OPAdsResult::SHOWED ||
            result == OPAdsResult::SUCCESS)
            event.send();
    });
}

void OPAds::showRewarded(const SEL_RewardedHandler& handler)
{
    assert(handler);
    
    if (!initialized)
    {
        handler(OPAdsResult::UNINITIALIZED);
        return;
    }

    OPCrashAnalytics::shared()->log("OPAds", "Show rewarded ad.");

    if (showingInterstitial || showingRewarded) 
    {
        OPCrashAnalytics::shared()->log("OPAds", "Not showing interstitial because another ad is already being showed.");
        
        handler(OPAdsResult::IS_ALREADY_SHOWING_AN_AD);
        return;
    }

    showingRewarded = true;

    _showRewarded([this, handler](const int code)
    {
        auto result = getResult(code);
        //Showed
        if (result == OPAdsResult::SHOWED){
            if (handler)
            {
                OPCocosThread::invoke("rewarded_call_handler", [this, handler, result]()
                {
                    handler(result);
                });
            }
            return;
        }
        OPLoadingView::shared()->hide();
        OPApplication::shared()->getAudioEngine()->resume();

        
        if (handler) 
        {
            OPCocosThread::invoke("rewarded_call_handler", [this, handler, result]()
            {
                //showingRewarded = false;
                handler(result);
            });
        }
        
        showingRewarded = false;

        if (result != OPAdsResult::SHOWED &&
            result != OPAdsResult::SUCCESS &&
            result != OPAdsResult::SHOWED_BUT_FAILED)
        {
            OPCrashAnalytics::shared()->warn("OPAds", "Rewarded was NOT shown... May be not ready. Code: " + std::to_string(code));
        }
        else
        {
            OPCrashAnalytics::shared()->log("OPAds", "Rewarded completion was called: " + std::to_string(code));
            interstitialTimer.restart();
        }
    });
    

    OPCrashAnalytics::shared()->log("OPAds", "Rewarded was shown! Waiting for completion.");

    OPApplication::shared()->getAudioEngine()->pause();
    OPLoadingView::shared()->show();
}

void OPAds::dispatchBannerVisibilityChangeEvent()
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(OPEvents::bannerVisibilityDidChange);
}

void OPAds::showBanner(OPBannerPosition position)
{
    if (!initialized)
        return;

    OPCrashAnalytics::shared()->log("OPAds", "Show banner.");

    if (!bannerEnabled) 
    {
        OPCrashAnalytics::shared()->log("OPAds", "Not showing banner because it is disabled.");
        return;
    }

    bannerPosition = position;
    wantsBanner = true;

    _showBanner(position);
    
    dispatchBannerVisibilityChangeEvent();
}

void OPAds::hideBanner()
{
    if (!initialized)
        return;

    OPCrashAnalytics::shared()->log("OPAds", "Hide banner.");

    wantsBanner = false;
    _hideBanner();
    
    dispatchBannerVisibilityChangeEvent();
}

bool OPAds::isBannerWanted()
{
    if (!initialized)
        return false;

    return wantsBanner;
}

void OPAds::showBanner()
{
    if (!initialized)
        return;

    showBanner(bannerPosition);
}

void OPAds::showInterstitialIfNeeded(const std::string& trigger, const SEL_InterstitialHandler& handler, bool ignoresMinInterval)
{
    assert(handler);
    
    if (!initialized)
    {
        handler(OPAdsResult::UNINITIALIZED);
        return;
    }

    OPCrashAnalytics::shared()->log("OPAds", "Show interstitial if needed = " + trigger);
    
    if (!interstitialEnabled) 
    {
        OPCrashAnalytics::shared()->log("OPAds", "Interstitial ignored because interstitial ads are disabled");
        
        handler(OPAdsResult::DISABLED);
        return;
    }

    auto count = 1;
    auto max = OPRemoteConfig::shared()->getInt("interstitial_" + trigger);

    if (max <= 0) { 
        handler(OPAdsResult::DISABLED);
        return;
    }

    auto it = interstitialTriggers.find(trigger);
    if (it == interstitialTriggers.end()) {
        interstitialTriggers[trigger] = 1;
    }
    else 
    {
        count = it->second + 1;
        interstitialTriggers[trigger] = count;
    }

    OPCrashAnalytics::shared()->log("OPAds", "Interstitial " + trigger + " (" + std::to_string(count) + ", " + std::to_string(max) + ")");

    if (count < max)
    {
        OPCrashAnalytics::shared()->log("OPAds", "Interstitial NOT showed");
        handler(OPAdsResult::TEMPORARILY_DISABLED);
        return;
    }

    showInterstitial([this, trigger, handler](const OPAdsResult result) {
        if (result == OPAdsResult::SHOWED ||
            result == OPAdsResult::SHOWED_BUT_FAILED)
        {
            OPCrashAnalytics::shared()->log("OPAds", "Interstitial showed");
            interstitialTriggers[trigger] = 0;
        }

        if (handler)
            handler(result);
    }, ignoresMinInterval);
}

void OPAds::increaseInterstitialCount()
{
    setTotalInterstitialCount(getTotalInterstitialCount() + 1);
}

void OPAds::initAdsResultMap()
{
    adsResultMap.emplace(-2000, OPAdsResult::UNINITIALIZED);
    adsResultMap.emplace(-1000, OPAdsResult::FAILED_UNKNOWN);
    adsResultMap.emplace(-100, OPAdsResult::SHOWED_BUT_FAILED);
    adsResultMap.emplace(-2, OPAdsResult::SHOWED_BUT_INCOMPLETE);
    adsResultMap.emplace(-1, OPAdsResult::SUCCESS);
    adsResultMap.emplace(0, OPAdsResult::SHOWED);
    adsResultMap.emplace(508, OPAdsResult::INIT_FAILED);
    adsResultMap.emplace(509, OPAdsResult::NO_ADS_TO_SHOW);
    adsResultMap.emplace(510, OPAdsResult::FAILED_NO_FILL_RATE);
    adsResultMap.emplace(520, OPAdsResult::FAILED_NO_CONNECTION);
    adsResultMap.emplace(1022, OPAdsResult::IS_ALREADY_SHOWING_AN_AD);
    adsResultMap.emplace(1023, OPAdsResult::NO_ADS_TO_SHOW);
    adsResultMap.emplace(1036, OPAdsResult::IS_ALREADY_SHOWING_AN_AD);
}

OPAdsResult OPAds::getResult(const int value)
{
    auto itr = adsResultMap.find(value);
    if (itr != adsResultMap.end())
        return itr->second;
    
    return OPAdsResult::FAILED_UNKNOWN;
}

bool OPAds::isBannerVisible()
{
    if (!initialized)
        return false;
    return _isBannerVisible();
}

bool OPAds::isInterstitialReady()
{
    if (!initialized)
        return false;
    return _isInterstitialReady();
}

bool OPAds::isRewardedReady()
{
    if (!initialized)
        return false;
    return _isRewardedReady();
}

void OPAds::setBannerBackgroundColor(cocos2d::Color4B color)
{
    if (!initialized)
        return;
    _setBannerBackgroundColor(color);
}

float OPAds::getScreenHeightFactor()
{
    if (!initialized)
        return 0;
    return _getScreenHeightFactor();
}

float OPAds::getBannerHeight()
{
    if (!initialized)
        return 0;
    return _getBannerHeight();
}

float OPAds::getBannerWidth()
{
    if (!initialized)
        return 0;
    return _getBannerWidth();
}

int OPAds::getInterstitialCount(const std::string& trigger)
{
    auto itr = interstitialTriggers.find(trigger);
    if (itr == interstitialTriggers.end())
        return 0;
    
    return itr->second;
}

bool OPAds::isInitialized() {
    return initialized;
}

void OPAds::onInterstitialLoaded()
{
    OPEventBus::shared()->notify(InterstitialLoaded());
}
