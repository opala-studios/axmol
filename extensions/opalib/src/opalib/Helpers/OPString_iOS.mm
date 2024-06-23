#include "./OPString.hpp"
#include <Foundation/Foundation.h>

std::string OPString::uppercased(const std::string& str)
{
    return [[NSString stringWithUTF8String:str.c_str()] uppercaseString].UTF8String;
}

std::string OPString::lowercased(const std::string& str)
{
    return [[NSString stringWithUTF8String:str.c_str()] lowercaseString].UTF8String;
}
