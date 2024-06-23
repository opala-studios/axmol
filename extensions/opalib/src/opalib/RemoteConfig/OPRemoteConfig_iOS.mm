#include "OPRemoteConfig_iOS.hpp"
#include "OPRemoteConfig_iOSImpl.h"
#include "OPCocosThread.hpp"

void OPRemoteConfig_iOS::setup(const std::string& defaultsPath)
{
    impl = (void*)[[OPRemoteConfig_iOSImpl alloc] init];
    [((OPRemoteConfig_iOSImpl*)impl) setupWithDefaultsPath:[NSString stringWithUTF8String:defaultsPath.c_str()]];
}

bool OPRemoteConfig_iOS::getBool(const std::string& name)
{
    return [((OPRemoteConfig_iOSImpl*)impl) getBool:[NSString stringWithUTF8String:name.c_str()]];
}

long OPRemoteConfig_iOS::getLong(const std::string& name)
{
    return [((OPRemoteConfig_iOSImpl*)impl) getLong:[NSString stringWithUTF8String:name.c_str()]];
}

double OPRemoteConfig_iOS::getDouble(const std::string& name)
{
    return [((OPRemoteConfig_iOSImpl*)impl) getDouble:[NSString stringWithUTF8String:name.c_str()]];
}

std::string OPRemoteConfig_iOS::getString(const std::string& name)
{
    return [[((OPRemoteConfig_iOSImpl*)impl) getString:[NSString stringWithUTF8String:name.c_str()]] UTF8String];
}

void OPRemoteConfig_iOS::_requestFetch()
{
    [((OPRemoteConfig_iOSImpl*)impl) fetchAndActivate:^(bool success)
    {
        OPCocosThread::invoke("remote_config_fetched", [success] {
            OPRemoteConfig::shared()->fetchWasCompleted(success);
        });
    }];
}
