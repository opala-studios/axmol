//
// Created by Opala Teste on 14/04/21.
//
#include "OPPlatform.hpp"
#include "platform/android/jni/JniHelper.h"
#include <cocos2d.h>

int OPPlatform::getSystemVersion()
{
    return cocos2d::JniHelper::callStaticIntMethod("com/opalastudios/opalib/helpers/PlatformHelper", "getSystemVersion");
}

int OPPlatform::getVersionCode()
{
    return cocos2d::JniHelper::callStaticIntMethod("com/opalastudios/opalib/helpers/PlatformHelper", "getVersionCode");
}