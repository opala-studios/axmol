#include "OPCrashAnalytics_Android.hpp"
#include <jni/JniHelper.h>

USING_NS_CC;

void OPCrashAnalytics_Android::setup()
{
}

void OPCrashAnalytics_Android::verb(const std::string& message)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/crashanalytics/CrashAnalytics", "verbose", message);
}

void OPCrashAnalytics_Android::log(const std::string& message)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/crashanalytics/CrashAnalytics", "log", message);
}

void OPCrashAnalytics_Android::warn(const std::string& message)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/crashanalytics/CrashAnalytics", "warning", message);
}

void OPCrashAnalytics_Android::error(const std::string& message)
{
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/crashanalytics/CrashAnalytics", "error", message);
}

void OPCrashAnalytics_Android::recordException(const std::string &message) {
    JniHelper::callStaticVoidMethod("com/opalastudios/opalib/crashanalytics/CrashAnalytics", "recordException", message);
}
