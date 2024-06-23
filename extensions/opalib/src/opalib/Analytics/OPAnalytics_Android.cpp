//
//  OPAnalytics_Android.cpp
//  Opalib
//
//  Created by Adilson Tavares on 29/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "./OPAnalytics_Android.hpp"

#include "./OPAnalyticsEvent.hpp"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

OPAnalytics_Android::OPAnalytics_Android()
{
}

void OPAnalytics_Android::setup()
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/analytics/Analytics", "initialize");
}

void OPAnalytics_Android::sendEvent(const OPAnalyticsEvent &event)
{
    OPAnalytics::sendEvent(event);

    auto env = JniHelper::getEnv();
    auto eventClass = env->FindClass("com/opalastudios/opalib/analytics/AnalyticsEvent");
    auto eventConstructor = env->GetMethodID(eventClass, "<init>", "(Ljava/lang/String;)V");

    jstring name = env->NewStringUTF(event.getName().c_str());
    jobject nativeEvent = env->NewObject(eventClass, eventConstructor, name);
    env->DeleteLocalRef(name);

    auto params = event.getParameters();
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        auto param = *it;
        auto value = param.second;

        if (value.getType() == OPParameter::Type::UNKNOWN) {
            continue;
        }

        jstring paramName = env->NewStringUTF(param.first.c_str());
        jmethodID paramMethod;

        switch (value.getType())
        {
            case OPParameter::Type::BOOL:
            {
                paramMethod = env->GetMethodID(eventClass, "setParam", "(Ljava/lang/String;Z)V");
                env->CallVoidMethod(nativeEvent, paramMethod, paramName, value.getBool());
                break;
            }

            case OPParameter::Type::INT:
            {
                paramMethod = env->GetMethodID(eventClass, "setParam", "(Ljava/lang/String;I)V");
                env->CallVoidMethod(nativeEvent, paramMethod, paramName, value.getInt());
                break;
            }

            case OPParameter::Type::FLOAT:
            case OPParameter::Type::DOUBLE:
            {
                paramMethod = env->GetMethodID(eventClass, "setParam", "(Ljava/lang/String;D)V");
                env->CallVoidMethod(nativeEvent, paramMethod, paramName, value.getDouble());
                break;
            }   

            case OPParameter::Type::STRING:
            {
                paramMethod = env->GetMethodID(eventClass, "setParam", "(Ljava/lang/String;Ljava/lang/String;)V");
                auto str = env->NewStringUTF(value.getCString());
                env->CallVoidMethod(nativeEvent, paramMethod, paramName, str);
                env->DeleteLocalRef(str);
                break;
            }

            default: break;
        }

        env->DeleteLocalRef(paramName);
    }

    auto sendMethod = env->GetMethodID(eventClass, "send", "()V");
    env->CallVoidMethod(nativeEvent, sendMethod);

    env->DeleteLocalRef(nativeEvent);
    env->DeleteLocalRef(eventClass);
}

void OPAnalytics_Android::setUserProperty(const std::string& key, const std::string& value) {
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/analytics/Analytics", "setUserProperty", key, value);
}
