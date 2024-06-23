#include "Opalib.h"

#include <JuceHeader.h>
#include <jni/JniHelper.h>
#include <juce_core/native/juce_android_JNIHelpers.h>


USING_NS_CC;

std::string Opalib::getPackageName()
{
    return JniHelper::callStaticStringMethod("com/opalastudios/opalib/app/Application", "getPackageName");
}

std::string Opalib::getAppVersion()
{
    return JniHelper::callStaticStringMethod("com/opalastudios/opalib/app/Application", "getAppVersion");
}

void cocos_android_app_init(JNIEnv* env) __attribute__((weak));


extern "C"{
    JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        JniHelper::setJavaVM(vm);

        cocos_android_app_init(JniHelper::getEnv());
        juce::JNI_OnLoadCustom(vm,reserved);
        return JNI_VERSION_1_4;
    }
}

