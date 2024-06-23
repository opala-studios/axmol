//
//  OPAudioManager.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAudioManager.hpp"
#include <cocos2d.h>
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "../Application/OPApplication.hpp"

OPAudioManager::OPAudioManager() : _playSoundWhenSilenced(true)
{
}

OPAudioManager* OPAudioManager::shared()
{
    static auto instance = new OPAudioManager();
    return instance;
}

void OPAudioManager::initialize()
{
    updateSilentPlaying();
}

void OPAudioManager::setPlaySoundWhenSilenced(bool play)
{
    if (play != _playSoundWhenSilenced)
    {
        _playSoundWhenSilenced = play;
        updateSilentPlaying();
    }
}

bool OPAudioManager::getPlaySoundWhenSilenced() const
{
    return _playSoundWhenSilenced;
}
