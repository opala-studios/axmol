//
//  OPDeviceSounds_iOS.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 01/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPDeviceSounds.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "../Debug/OPDebug.hpp"

#import "OPDeviceSounds_iOSImpl.h"
#import <AVFoundation/AVFoundation.h>
#import <StoreKit/StoreKit.h>

bool OPDeviceSounds::isAvailable()
{
    if (@available(iOS 11.0, *)) {
        return true;
    }
    
    return false;
}

void OPDeviceSounds::requestSingle(const SEL_RequestSingleSoundHandler& receivedCompletion)
{
    if (!isAvailable())
    {
        if (receivedCompletion) {
            receivedCompletion(NULL);
        }
        return;
    }
    
    // Don't delete this. The completion must be copied to a local variable so it
    // can keep a strong reference to the copied callback. Otherwise, the completion
    // would be released and a EXC_BAD_ACCESS would be thrown when calling the block.
    SEL_RequestSingleSoundHandler completion = receivedCompletion;
    
    OPDeviceSounds_iOSImpl* impl = [[OPDeviceSounds_iOSImpl alloc] init];
    [impl showWithCompletion:^(NSURL* url)
    {
        OPDeviceSound* result = NULL;
        if (url != NULL)
        {
            [url startAccessingSecurityScopedResource];
            
            NSString* documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
            NSString* soundsDirectory = [documents stringByAppendingPathComponent:@"temp/devicesounds"];
            NSString* destinationPath = [soundsDirectory stringByAppendingPathComponent:url.lastPathComponent];
            
            if (![NSFileManager.defaultManager fileExistsAtPath:soundsDirectory]) {
                [NSFileManager.defaultManager createDirectoryAtPath:soundsDirectory withIntermediateDirectories:YES attributes:nil error:nil];
            }
            
            NSError* error;
            [NSFileManager.defaultManager copyItemAtPath:url.path toPath:destinationPath error:&error];
            
            result = OPDeviceSound::create();
            result->setName(destinationPath.lastPathComponent.stringByDeletingPathExtension.UTF8String);
            result->setFilePath(destinationPath.UTF8String);
            result->retain();
            
            [url stopAccessingSecurityScopedResource];
        }

        OPCocosThread::invoke("device_sounds_single_completion", [completion, result]()
        {
            if (completion) {
                completion(result);
            }
            
            if (result != nullptr) {
                result->release();
            }
        });
    }];
}
