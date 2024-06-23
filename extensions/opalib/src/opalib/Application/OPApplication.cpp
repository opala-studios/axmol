
//
//  OPApplication.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPApplication.hpp"
#include "../Opalib.h"
#include "../Events/OPEvents.hpp"
//#include <Adjust/Adjust2dx.h>
#include <Managers/OPAppManager.hpp>
#include <opalib/Audio/Cricket/OPCricketEngine.hpp>
#include <opalib/Audio/Juce/OPJuceEngine.hpp>

USING_NS_CC;

OPApplication* OPApplication::shared()
{
    return dynamic_cast<OPApplication*>(OPApplication::getInstance());
}

OPApplication::OPApplication()
{
    _audioEngine = std::make_unique<OPAudioEngine>();
    _directorValidator = std::make_unique<opalib::Validator>([](){ return cocos2d::Director::getInstance()->isValid(); });
    _vibrator = std::unique_ptr<opalib::Vibrator>(opalib::Vibrator::createForPlatform());
}

OPAudioEngine* OPApplication::getAudioEngine()
{
    return _audioEngine.get();
}

opalib::Validator *OPApplication::getDirectorValidator()
{
    return _directorValidator.get();
}

opalib::Vibrator *OPApplication::getVibrator()
{
    return _vibrator.get();
}

OPApplication::~OPApplication()
{
    OP_LOG("BEGIN");
    Opalib::end();
    _audioEngine.reset();
    _directorValidator.reset();
    OP_LOG("END");
}

void OPApplication::initGLContextAttrs()
{
    OP_LOG("BEGIN");
    Opalib::shared()->initGLContextAttrs();
    OP_LOG("END");
}

bool OPApplication::applicationDidFinishLaunching()
{
    OP_LOG("BEGIN");

    prepareCustomInitializers();
    Opalib::shared()->initialize();
    createAppManager();
    Opalib::shared()->start(OPAppManager::shared()->getOpalaSplashScene());
    OP_LOG("END");
    return true;
}

void OPApplication::applicationDidEnterBackground()
{
    OP_LOG("BEGIN");
    Opalib::shared()->applicationDidEnterBackground();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    Adjust2dx::onPause();
#endif
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(OPEvents::applicationDidEnterBackground);
    OP_LOG("END");
}

void OPApplication::applicationWillEnterForeground()
{
    OP_LOG("BEGIN");
    Opalib::shared()->applicationWillEnterForeground();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Adjust2dx::onResume();
#endif
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(OPEvents::applicationWillEnterForeground);
    _directorValidator->flush();
    OP_LOG("END");
}

void OPApplication::applicationWillResignActive()
{
	OP_LOG("BEGIN");
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(OPEvents::applicationWillResignActive);
	OP_LOG("END");
}

void OPApplication::applicationDidBecomeActive()
{
	OP_LOG("BEGIN");
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(OPEvents::applicationDidBecomeActive);
	OP_LOG("END");
}

void OPApplication::prepareCustomInitializers()
{
}


