//
//  OPInitializer.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 30/07/19.
//

#include <cocos2d/cocos/network/HttpClient.h>
#include "OPInitializer.hpp"
#include "../Application/OPApplication.hpp"
#include "../Debug/OPDebug.hpp"
#include "../UserData/OPUserData.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "../Loading/OPLoadingView.hpp"
#include "../Analytics/OPAnalytics.hpp"
#include "../Notifications/OPNotificationManager.hpp"
#include "../Alerts/OPNativeAlert.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "../Audio/OPAudioManager.hpp"
#include "../Ads/OPAds.hpp"
#include "../Adjust/OPAdjust.hpp"
#include "../Localization/OPLocalization.hpp"
#include "../RemoteConfig/OPRemoteConfig.hpp"
#include "../SoundEffects/OPSoundController.h"
#include "../Notifications/OPLocalNotificationManager.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "./OPInitializer_iOS.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "./OPInitializer_Android.hpp"
#endif

OPInitializer* OPInitializer::shared()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static OPInitializer* instance = new OPInitializer_iOS();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static OPInitializer* instance = new OPInitializer_Android();
#endif
    
    return instance;
}

void OPInitializer::initialize()
{
    OPDebug::log("Initializer", "Initializing");
    callInitializationHandlers(_preInitializationHandlers);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(6);
    cocos2d::network::HttpClient::getInstance()->setTimeoutForRead(6);
    OPNativeAlert::shared()->initialize();
    OPUserData::shared()->initialize();
    OPLocalization::shared()->initialize();
    OPCocosThread::initialize();
    OPApplication::shared()->getAudioEngine()->initialize();
    OPAudioManager::shared()->initialize();
    OPSoundController::shared()->initialize();
    OPLoadingView::shared()->initialize();
    OPAnalytics::shared()->initialize();
    OPRemoteConfig::shared()->initialize();
    OPNotificationManager::shared()->initialize();
    OPLocalNotificationManager::shared()->initialize();
    callInitializationHandlers(_postInitializationHandlers);
}

void OPInitializer::addPreInitialization(const SEL_InitializationHandler& callback)
{
    _preInitializationHandlers.push_back(callback);
}

void OPInitializer::addPostInitialization(const SEL_InitializationHandler& callback)
{
    _postInitializationHandlers.push_back(callback);
}

void OPInitializer::callInitializationHandlers(std::vector<SEL_InitializationHandler>& handlers)
{
    for (const auto& handler : handlers) {
        handler();
    }
    
    handlers.clear();
}

bool OPInitializer::needsIDFAConfirmation() {
    return false;
}

void OPInitializer::askForIDFAConfirmation(const std::function<void(bool)> &onCompletion) {
    onCompletion(false);//do nothing
}

