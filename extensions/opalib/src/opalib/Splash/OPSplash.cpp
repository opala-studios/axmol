//
//  OPSplash.cpp
//  Opalib
//
//  Created by Adilson Tavares on 16/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPSplash.hpp"
#include "../Analytics/OPAnalytics.hpp"
#include "../Analytics/OPAnalyticsEvent.hpp"

USING_NS_CC;

OPSplash* OPSplash::create(Scene* initialScene)
{
    auto splash = OPSplash::create();
    splash->initialScene = initialScene;
    
    return splash;
}

bool OPSplash::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    return true;
}

void OPSplash::onEnter()
{
    Scene::onEnter();
    OPAnalytics::shared()->setCurrentScreen("splash");
}
