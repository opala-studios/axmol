#include "OPString.hpp"
#include "platform/android/jni/JniHelper.h"
#include <cocos2d.h>
#include <boost/algorithm/string.hpp>

USING_NS_CC;

std::string OPString::lowercased(const std::string& str)
{
//    return boost::algorithm::to_lower_copy(str);
    return JniHelper::callStaticStringMethod("com/opalastudios/opalib/helpers/StringHelper", "toLower", str);
}

std::string OPString::uppercased(const std::string& str)
{
//    return boost::algorithm::to_upper_copy(str);
    return JniHelper::callStaticStringMethod("com/opalastudios/opalib/helpers/StringHelper", "toUpper", str);
}