#include "OPRemoteConfig.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Opalib.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OPRemoteConfig_Android.hpp"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "OPRemoteConfig_iOS.hpp"
#endif

USING_NS_CC;

OPRemoteConfig* OPRemoteConfig::shared()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static auto instance = new OPRemoteConfig_Android();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static auto instance = new OPRemoteConfig_iOS();
#endif

    return instance;
}

void OPRemoteConfig::requestFetch(const SEL_FetchHandler& handler)
{
    fetchCompletion = handler;
    _requestFetch();
}

void OPRemoteConfig::fetchWasCompleted(bool success)
{
    OPDebug::log("RemoteConfig", "Fetch was completed with success = " + std::to_string(success));

    OPEventBus::shared()->notify(opalib::events::OnRemoteConfigFetched{success});

    _fetchStatus = success ? Initialized : FailedToInitialize;

    if (fetchCompletion)
    {
        fetchCompletion(success);
        fetchCompletion = 0;
    }
}

void OPRemoteConfig::initialize()
{
    auto defaultsPath = FileUtils::getInstance()->fullPathForFilename("config/remote_config_defaults.json");
    setup(defaultsPath);
    requestFetch(nullptr);
    _fetchStatus = Initializing;
}

OPFetchStatus OPRemoteConfig::getFetchStatus() const
{
    return _fetchStatus;
}
