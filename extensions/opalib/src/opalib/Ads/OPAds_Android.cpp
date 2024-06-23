//
//  OPAds_Android.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 04/07/19.
//

#include "OPAds_Android.hpp"
#include "../Misc/OPCocosThread.hpp"
#include <opalib/Opalib.h>

USING_NS_CC;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_ads_AdsManager_onInterstitialDone(JNIEnv *env, jobject thisObj, jint code)
    {
        OPCocosThread::invoke("interstitial_completion", [code]
        {
            OPAds_Android *manager = (OPAds_Android*)OPAds::shared();
            manager->onInterstitialDone(code);
        });
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_ads_AdsManager_onRewardedDone(JNIEnv *env, jobject thisObj, jint code)
    {
        OPCocosThread::invoke("rewarded_completion", [code]
        {
            OPAds_Android *manager = (OPAds_Android*)OPAds::shared();
            manager->onRewardedDone(code);
        });
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_ads_AdsManager_onBannerLoad(JNIEnv *env, jobject thisObj)
    {
        OPCocosThread::invoke("banner_loaded", []
        {
            OPAds_Android *manager = (OPAds_Android*)OPAds::shared();
            manager->dispatchBannerVisibilityChangeEvent();
        });
    }

    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_ads_AdsManager_onInterstitialLoad(JNIEnv *env, jobject thisObj)
    {
        OPCocosThread::invoke("interstitial_loaded", []
        {
            OPAds_Android *manager = (OPAds_Android*)OPAds::shared();
            manager->onInterstitialLoaded();
        });
    }
}

OPAds_Android::OPAds_Android()
{
}

float OPAds_Android::_getBannerHeight()
{
    float percent = JniHelper::callStaticFloatMethod("com/opalastudios/opalib/ads/AdsManager", "getBannerHeightPercent");
    return percent * Director::getInstance()->getVisibleSize().height;
}

float OPAds_Android::_getBannerWidth()
{
    float percent = JniHelper::callStaticFloatMethod("com/opalastudios/opalib/ads/AdsManager", "getBannerWidthPercent");
    return percent * Director::getInstance()->getVisibleSize().width;
}

float OPAds_Android::_getScreenHeightFactor()
{
    float height = JniHelper::callStaticFloatMethod("com/opalastudios/opalib/ads/AdsManager", "getScreenHeightInPixels");
    return height / Director::getInstance()->getVisibleSize().height;;
}

void OPAds_Android::_setBannerBackgroundColor(cocos2d::Color4B color)
{
    auto colorHex = OPColor::toHex(color);
    OPDebug::log("OPAds_Android", "Color code = " + colorHex);

    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/ads/AdsManager", "setBannerBackgroundColor", colorHex);
}

void OPAds_Android::onInterstitialDone(int code)
{
    if (_interstitialCompletion)
    {
        _interstitialCompletion(code);
        //_interstitialCompletion = nullptr;
    }
}

void OPAds_Android::onRewardedDone(int code)
{
    if (_rewardedCompletion)
    {
        _rewardedCompletion(code);
        //_rewardedCompletion = nullptr;
    }
}

void OPAds_Android::setup(OPAdsConfig* config)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/ads/AdsManager", "setup", configFilePath, config->userConsent);
}

void OPAds_Android::_showBanner(OPBannerPosition position)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/ads/AdsManager", "showBanner", position.getName());
}

void OPAds_Android::_hideBanner()
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/ads/AdsManager", "hideBanner");
}

void OPAds_Android::_showInterstitial(const SEL_InterstitialHandlerWithCode& handler)
{
    _interstitialCompletion = handler;

    JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/ads/AdsManager", "showInterstitial");
}

void OPAds_Android::_showRewarded(const SEL_RewardedHandlerWithCode& handler)
{
    _rewardedCompletion = handler;

    JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/ads/AdsManager", "showRewarded");
}

bool OPAds_Android::_isBannerVisible()
{
    return JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/ads/AdsManager", "isBannerVisible");
}

bool OPAds_Android::_isInterstitialReady()
{
    return JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/ads/AdsManager", "isInterstitialReady");
}

bool OPAds_Android::_isRewardedReady()
{
    return JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/ads/AdsManager", "isRewardedReady");
}
