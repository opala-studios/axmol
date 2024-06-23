#include "OPCrashAnalytics.hpp"
#include "../Debug/OPDebug.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPCrashAnalytics_Android.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPCrashAnalytics_iOS.hpp"
#endif

OPCrashAnalytics* OPCrashAnalytics::shared()
{
    static OPCrashAnalytics* instance = 0;
    
    if (!instance)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        instance = new OPCrashAnalytics_Android();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        instance = new OPCrashAnalytics_iOS();
#endif
        instance->setup();
    }

    return instance;
}

void OPCrashAnalytics::verb(const std::string& context, const std::string& message)
{
    auto text = context + " | " + message;
    verb(text);
}

void OPCrashAnalytics::log(const std::string& context, const std::string& message)
{
    auto text = context + " | " + message;
    log(text);
}

void OPCrashAnalytics::warn(const std::string& context, const std::string& message)
{
    auto text = context + " | " + message;
    warn(text);
}

void OPCrashAnalytics::error(const std::string& context, const std::string& message)
{
    auto text = context + " | " + message;
    error(text);
}

void OPCrashAnalytics::recordException(const std::string &context, const std::exception &e)
{
    auto text = context + " | " + e.what();
    recordException(text);
}