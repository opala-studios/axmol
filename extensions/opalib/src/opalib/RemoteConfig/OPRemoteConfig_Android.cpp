#include "OPRemoteConfig_Android.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "../Opalib.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_opalastudios_opalib_remoteconfig_RemoteConfig_remoteConfigDidFetch(JNIEnv *env, jobject thisObj, jboolean success)
    {
        OPCocosThread::invoke("remote_config_fetched", [success] {
            OPRemoteConfig::shared()->fetchWasCompleted(success);
        });
    }
}

void OPRemoteConfig_Android::setup(const std::string& defaultsPath)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/remoteconfig/RemoteConfig", "setup", Opalib::shared()->isDebug(), defaultsPath);
}

void OPRemoteConfig_Android::_requestFetch()
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/remoteconfig/RemoteConfig", "requestFetch");
}

bool OPRemoteConfig_Android::getBool(const std::string& name)
{
    return JniHelper::callStaticBooleanMethod("com/opalastudios/opalib/remoteconfig/RemoteConfig", "getBoolean", name);
}

long OPRemoteConfig_Android::getLong(const std::string& name)
{ 
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/opalastudios/opalib/remoteconfig/RemoteConfig", "getLong", "(Ljava/lang/String;)J")) 
    {
        jstring jname = info.env->NewStringUTF(name.c_str());
        auto result = info.env->CallStaticLongMethod(info.classID, info.methodID, jname);
        info.env->DeleteLocalRef(jname);
        info.env->DeleteLocalRef(info.classID);

        return result;
    }

    return 0;
}

double OPRemoteConfig_Android::getDouble(const std::string& name)
{
    return JniHelper::callStaticDoubleMethod("com/opalastudios/opalib/remoteconfig/RemoteConfig", "getDouble", name);
}

std::string OPRemoteConfig_Android::getString(const std::string& name) 
{
    return JniHelper::callStaticStringMethod("com/opalastudios/opalib/remoteconfig/RemoteConfig", "getString", name);
}
