//
//  OPDeviceSounds.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 01/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPDeviceSounds_hpp
#define OPDeviceSounds_hpp

#include <cocos2d.h>
#include "./OPDeviceSound.hpp"

class OPDeviceSounds
{
public:    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    typedef std::function<void(OPDeviceSound*)> SEL_RequestSingleSoundHandler;
    static void requestSingle(const SEL_RequestSingleSoundHandler& completion);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    typedef std::function<void(cocos2d::Vector<OPDeviceSound*>)> SEL_RequestListSoundHandler;
    static void requestList(const SEL_RequestListSoundHandler& completion);
#endif
    
    static bool isAvailable();
};

#endif /* OPDeviceSounds_hpp */
