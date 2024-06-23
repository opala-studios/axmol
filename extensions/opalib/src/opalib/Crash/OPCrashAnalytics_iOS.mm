#include "OPCrashAnalytics_iOS.hpp"
#include "OPCrashAnalytics_iOSImpl.h"
#include "../../boost/include/boost/algorithm/string.hpp"

void OPCrashAnalytics_iOS::setup()
{
    impl = [[OPCrashAnalytics_iOSImpl alloc] init];
    [((OPCrashAnalytics_iOSImpl*)impl) setup];
}

void OPCrashAnalytics_iOS::verb(const std::string& message)
{
    [((OPCrashAnalytics_iOSImpl*)impl) verbose:[NSString stringWithUTF8String:message.c_str()]];
}

void OPCrashAnalytics_iOS::log(const std::string& message)
{
    [((OPCrashAnalytics_iOSImpl*)impl) log:[NSString stringWithUTF8String:message.c_str()]];
}

void OPCrashAnalytics_iOS::warn(const std::string& message)
{
    [((OPCrashAnalytics_iOSImpl*)impl) warning:[NSString stringWithUTF8String:message.c_str()]];
}

void OPCrashAnalytics_iOS::error(const std::string& message)
{
    [((OPCrashAnalytics_iOSImpl*)impl) error:[NSString stringWithUTF8String:message.c_str()]];
}

void OPCrashAnalytics_iOS::recordException(const std::string &message)
{
    [((OPCrashAnalytics_iOSImpl*)impl) recordException:[NSString stringWithUTF8String:message.c_str()]];
}
