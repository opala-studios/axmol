//
//  OPPermissions_iOS.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 08/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPPermissions.hpp"
#include "../Misc/OPCocosThread.hpp"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

void OPPermissions::requestMicrophone(const SEL_PermissionRequestHandler& handler)
{
    microphoneHandler = handler;
    [AVAudioSession.sharedInstance requestRecordPermission:^(BOOL granted)
    {
        OPCocosThread::invoke("microphone_permission_callback", [granted]()
        {
            if (OPPermissions::microphoneHandler)
            {
                OPPermissions::microphoneHandler(granted);
                OPPermissions::microphoneHandler = nullptr;
            }
        });
    }];
}

void OPPermissions::openAppSettings()
{
    [UIApplication.sharedApplication openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
}

bool OPPermissions::isMicrophoneAuthorized()
{
    return AVAudioSession.sharedInstance.recordPermission == AVAudioSessionRecordPermissionGranted;
}
