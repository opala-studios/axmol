//
//  Opalib.h
//  Opalib
//
//  Created by Adilson Tavares on 15/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef __OPALIB_H__
#define __OPALIB_H__

//#include "./API/OPBaseAPI.hpp"
//#include "./Adjust/OPAdjust.hpp"
//#include "./Ads/OPAds.hpp"
//#include "./Alerts/OPNativeAlert.hpp"
//#include "./Analytics/OPAnalytics.hpp"
//#include "./Analytics/OPAnalyticsEvent.hpp"
//#include "./Application/OPApplication.hpp"
//#include "./Audio/OPAudioManager.hpp"
//#include "./Audio/OPAudioTransform.hpp"
//#include "./Audio/OPAudioWriter.hpp"
//#include "./Audio/OPInputPitchEstimator.hpp"
//#include "./Audio/Juce/OPJuceEngine.hpp"
//#include "./Audio/OPJuceInput.hpp"
//#include "./Audio/OPMicRecorder.hpp"
//#include "./Audio/OPOutputRecorder.hpp"
//#include "./Audio/OPPlayer.hpp"
//#include "./Audio/OPSynthesizer.hpp"
//#include "./Audio/OPStreamedPlayer.hpp"
//#include "./Audio/OPMemoryPlayer.hpp"
//#include "./Audio/OPWaveform.hpp"
//#include "./Audio/OPWaveformRange.hpp"
//#include "./Buttons/OPButton.hpp"
//#include "./Config/OPAppConfig.hpp"
//#include "./Crash/OPCrashAnalytics.hpp"
//#include "./Datetime/OPTimer.hpp"
#include "./Debug/OPDebug.hpp"
//#include "./Events/OPEvents.hpp"
//#include "./Fairy/OPGRoot.hpp"
//#include "./Helpers/OPPlatform.hpp"
//#include "./Helpers/OPString.hpp"
//#include "./Initializer/OPInitializer.hpp"
//#include "./Json/OPJson.hpp"
//#include "./Parsers/OPJsonParser.hpp"
//#include "./Layout/OPScreenLayout.hpp"
//#include "./Loading/OPLoadingView.hpp"
//#include "./Localization/OPLocalization.hpp"
//#include "./Midi/OPMidi.h"
//#include "./Misc/OPCocosThread.hpp"
//#include "./Misc/OPColor.hpp"
//#include "./Misc/OPCrypto.hpp"
//#include "./Misc/OPDirectory.hpp"
//#include "./Misc/OPEvent.hpp"
//#include "./Misc/OPEventBus.hpp"
//#include "./Misc/OPFairySafeArea.hpp"
//#include "./Misc/OPFile.hpp"
//#include "./Misc/OPMacros.hpp"
//#include "./Misc/OPPath.hpp"
//#include "./Misc/OPSafeArea.hpp"
//#include "./Misc/OPTouch.hpp"
//#include "./Misc/OPWindow.hpp"
//#include "./Misc/OPZip.hpp"
//#include "./Models/OPCategory.hpp"
//#include "./Models/OPGroupCategory.hpp"
//#include "./Models/OPKit.hpp"
//#include "./Models/OPKitCategory.hpp"
//#include "./Native/OPDeviceSounds.hpp"
//#include "./Navigation/OPPopScene.hpp"
//#include "./Network/OPDownloader.hpp"
#include "./Pooling/OPPooling.hpp"
//
//#include "./TestManager/OPTestManager.hpp"
//#include "./TestManager/OPTestEvent.hpp"
//#include "./TestManager/OPTest.hpp"
//
//#include "./Midi/OPMidi.h"
//#include "./Audio/OPMicRecorder.hpp"
//#include "./Audio/OPOutputRecorder.hpp"
//#include "./Audio/OPJuceInput.hpp"
//#include "./Audio/OPAudioFormat.hpp"
//#include "./Audio/OPInputPitchEstimator.hpp"
//
//#include "./Analytics/OPAnalytics.hpp"
//#include "./Analytics/OPAnalyticsEvent.hpp"
//
//#include "./Nodes/OPDrawNode.hpp"
//#include "./Notifications/OPNotificationManager.hpp"
//#include "./Nodes/OPLineNode.hpp"
//#include "./Permissions/OPPermissions.hpp"
//#include "./RemoteConfig/OPRemoteConfig.hpp"
//#include "./RemoteConfig/OPRemoteConfig.hpp"
#include "./Scene/OPBaseScene.hpp"
//#include "./Share/OPShare.hpp"
//#include "./SoundEffects/OPSoundEffects.hpp"
//#include "./SoundEffects/OPSoundController.h"
//#include "./UI/OPFairyComponent.hpp"
//#include "./UI/OPPinchableComponent.hpp"
//#include "./UserData/OPUserData.hpp"
//#include "./Utils/FairyUtils.hpp"
//#include "./Utils/ComponentPool.hpp"
//
#include "./Spawner/SpawnerGObject.hpp"
//#include <Utils/PerformanceTracker.hpp>
//#include <Utils/OPDateTime.hpp>
#include <Notifications/OPLocalNotificationManager.hpp>
#include "cocos2d.h"

class Opalib
{
public:
    Opalib();
    ~Opalib();

    bool debugMode{false};

//    void setupViewForCocos();
    void setupViewForFairyGUI();
//    void setupViewForHybrid();
    
    int getXSize();
    bool isTablet();

public:
    static Opalib* shared();

    void setDebugMode(bool debug);
    bool isDebug() { return debugMode; }

    void setInitialScene(ax::Scene* scene);

    void initialize();
    void start(cocos2d::Scene* scene);
    static void end();

    void initGLContextAttrs();
    void applicationWillEnterForeground();
    void applicationDidEnterBackground();

    std::string getPackageName();
    std::string getAppVersion();

    bool isTabletResolution();

    CC_SYNTHESIZE_READONLY(bool, initialized, Initialized);
//    CC_SYNTHESIZE_READONLY(OPAppConfig*, appConfig, AppConfig);

protected:
    void setupDeviceTypeAndSize();

    int _xSize = 1;
    bool _isTabletResolution = false;
};

#endif
