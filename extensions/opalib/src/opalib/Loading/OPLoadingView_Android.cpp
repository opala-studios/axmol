//
//  OPLoadingView_Android.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 10/07/19.
//

#include "OPLoadingView_Android.hpp"
#include "platform/android/jni/JniHelper.h"

#include <cocos2d.h>

USING_NS_CC;

OPLoadingView_Android::OPLoadingView_Android()
{
}

void OPLoadingView_Android::initialize()
{
}

void OPLoadingView_Android::showView(const std::string& title)
{
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "showLoading", title.c_str());
}

void OPLoadingView_Android::hideView()
{
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "hideLoading");
}
