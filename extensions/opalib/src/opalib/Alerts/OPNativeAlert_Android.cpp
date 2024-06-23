//
//  OPNativeAlert_Android.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 01/08/19.
//

#include "OPNativeAlert_Android.hpp"
#include <jni/JniHelper.h>

USING_NS_CC;

OPNativeAlert_Android::OPNativeAlert_Android() : OPNativeAlert()
{
}

void OPNativeAlert_Android::setup()
{
    
}

void OPNativeAlert_Android::show(const std::string& title, const std::string& message, const std::string& buttonText, SEL_AlertCompletion completion)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/alert/Alert", "showMessage", title, message, buttonText);
}