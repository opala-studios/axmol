//
//  OPPlatform.cpp
//  Opalib
//
//  Created by Adilson Tavares on 12/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPPlatform.hpp"
#include <cocos2d.h>

const char* OPPlatform::id()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "ios";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "android";
#else
    return "unknown";
#endif
}

const char* OPPlatform::name()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "iOS";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "Android";
#else
    return "unknown";
#endif
}
