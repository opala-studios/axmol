//
//  OPAudioManager.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAudioManager_hpp
#define OPAudioManager_hpp

#include <cocos2d.h>
#include "eventbus/EventBus.h"
#include "eventbus/TokenHolder.h"
#include "../Application/OPApplication.hpp"

class OPAudioManager
{
public:
    
    static OPAudioManager* shared();
    
    OPAudioManager();
    
    void initialize();
    void updateSilentPlaying();

    void audioEngineDidPause();
    void audioEngineDidResume();

    CC_PROPERTY(bool, _playSoundWhenSilenced, PlaySoundWhenSilenced);
};

#endif /* OPAudioManager_hpp */
