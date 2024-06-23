//
//  OPAudioManager_iOS.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 02/09/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAudioManager.hpp"
#import <AVFoundation/AVFoundation.h>
#import <GoogleMobileAds/GoogleMobileAds.h>
//#include <cocos/audio/ios/CDAudioManager.h>

void OPAudioManager::updateSilentPlaying()
{
    if (_playSoundWhenSilenced) {
        [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback error: nil];
    }
    else {
        [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategorySoloAmbient error: nil];
    }
}

