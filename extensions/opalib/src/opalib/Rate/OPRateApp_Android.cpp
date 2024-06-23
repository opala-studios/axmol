//
// Created by Talles Borges  on 19/08/20.
//

#include "../Opalib.h"
#include "OPRateApp.hpp"

void OPRateApp::rate()
{
//    auto packageName = Opalib::shared()->getPackageName();
//    auto storeUrl = "http://play.google.com/store/apps/details?id=";
//    cocos2d::Application::getInstance()->openURL(storeUrl + packageName);
    cocos2d::JniHelper::callStaticVoidMethod("com/opalastudios/opalib/rate/RateApp", "rate");
}
