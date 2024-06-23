#include "./OPPlatform.hpp"
#include <Foundation/Foundation.h>

int OPPlatform::getSystemVersion()
{
    return [[UIDevice currentDevice].systemVersion floatValue];
}

int OPPlatform::getVersionCode()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] intValue];
}
