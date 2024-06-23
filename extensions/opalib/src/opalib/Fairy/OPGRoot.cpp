//
//  OPGRoot.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPGRoot.hpp"
#include <cocos2d.h>
#include "../Debug/OPDebug.hpp"
#include <opalib/SoundEffects/OPSoundController.h>

USING_NS_CC;
using namespace fairygui;

GRoot* OPGRoot::create(cocos2d::Scene* scene, int zOrder)
{
    OPGRoot* pRet = new (std::nothrow) OPGRoot();
    if (pRet && pRet->initWithScene(scene, zOrder))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void OPGRoot::playSound(PackageItem* item, float volume)
{
    if (volume <= 0 || !isSoundEnabled()) {
        return;
    }
    OP_LOG_VERB("Playing groot sound");

    auto path = FileUtils::getInstance()->fullPathForFilename(item->file);

    OPSoundController::shared()->addSound(item->file, path, [item, path](bool success) {
        if (!success){
            OP_LOG_ERROR("Failed to load sound for GRoot on path " + path);
            return;
        }
        OPSoundController::shared()->play(item->file);
    });
}

