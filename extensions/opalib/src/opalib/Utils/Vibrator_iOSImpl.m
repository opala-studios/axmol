//
//  Vibrator_iOSImpl.m
//  opalib-ios
//
//  Created by Opala Teste on 26/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#import "Vibrator_iOSImpl.h"
#include <AudioToolbox/AudioToolbox.h>

@implementation Vibrator_iOSImpl

- (void)vibratePop
{
    AudioServicesPlaySystemSoundWithCompletion(1520, NULL);
}

- (void) vibratePeek
{
    AudioServicesPlaySystemSoundWithCompletion(1519, NULL);
}

@end
