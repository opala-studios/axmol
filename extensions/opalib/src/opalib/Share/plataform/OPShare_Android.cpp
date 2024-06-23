//
//  OPShare.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 07/04/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "../OPShare.hpp"
#include "../../Debug/OPDebug.hpp"
#include <cocos2d.h>

void OPShare::shareFile(const char* filePath, cocos2d::Rect rect){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com.opalastudios.opalib.helpers.OPShare", "shareFile", "(Ljava/lang/String;)V")) {
        jstring jUrl = t.env->NewStringUTF(filePath);

        t.env->CallStaticVoidMethod(t.classID, t.methodID, jUrl);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jUrl);
    }
}

void OPShare::shareMessage(const char* message, cocos2d::Rect rect){
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com.opalastudios.opalib.helpers.OPShare", "shareMessage", "(Ljava/lang/String;)V")) {
        jstring jMessage = t.env->NewStringUTF(message);

        t.env->CallStaticVoidMethod(t.classID, t.methodID, jMessage);
        
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jMessage);
    }
}
