#include "../Opalib.h"
#include <Foundation/Foundation.h>

std::string Opalib::getPackageName()
{
    return NSBundle.mainBundle.bundleIdentifier.UTF8String;
}

std::string Opalib::getAppVersion()
{
    NSString* version = [NSBundle.mainBundle.infoDictionary objectForKey:@"CFBundleVersion"];
    return version.UTF8String;
}
