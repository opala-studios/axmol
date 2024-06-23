//
//  OPString.cpp
//  Opalib
//
//  Created by Adilson Tavares on 12/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPString.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <locale>
#include <codecvt>
#include <boost/locale.hpp>
#include <cocos2d.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>
#include <boost/lexical_cast/bad_lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

void OPString::trim(std::string& string, const std::string& characters)
{
    boost::trim_if(string, [characters](char c) {
        return characters.find(c) != std::string::npos;
    });
}

bool OPString::queryMatches(const std::string& query, const std::string& text)
{
    return contains(normalizeForQuery(text), normalizeForQuery(query));
}

std::string OPString::normalizeForQuery(const std::string& query)
{
    return lowercased(trimmed(query));
}

std::string OPString::captureStringArguments(const std::string input, const char* arg...)
{
    va_list arguments;
    int argumentNumber = 0;
    std::string::size_type cindex;
    std::string newText = input;

    for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *))
    {
        std::string capture = "{" + std::to_string(argumentNumber) + "}";
        
        while ((cindex = newText.find(capture.c_str())) != std::string::npos) {
            newText.replace(cindex, capture.size(), arg);
        }
        
        argumentNumber++;
    }

    va_end(arguments);

    return newText;
}

std::string OPString::captureStringArguments(const std::string input, const std::vector<std::string> &args)
{
    int argumentNumber = 0;
    std::string::size_type cindex;
    std::string newText = input;

    for (auto const& arg : args)
    {
        std::string capture = "{" + std::to_string(argumentNumber) + "}";

        while ((cindex = newText.find(capture.c_str())) != std::string::npos) {
            newText.replace(cindex, capture.size(), arg);
        }

        argumentNumber++;
    }
    
    return newText;
}

std::string OPString::trimmed(const std::string& string, const std::string& characters)
{
    return boost::trim_copy_if(string, [characters](char c) {
        return characters.find(c) != std::string::npos;
    });
}

void OPString::replace(std::string& string, const std::string& occurrence, const std::string& replacement)
{
    boost::replace_all(string, occurrence, replacement);
}

std::string OPString::replacing(const std::string& string, const std::string& occurrence, const std::string& replacement)
{
    return boost::replace_all_copy(string, occurrence, replacement);
}

std::string OPString::uuid()
{
    auto result = boost::uuids::random_generator()();
    return boost::lexical_cast<std::string>(result);
}

bool OPString::startsWith(const std::string& str, const std::string& start)
{
    if (start.size() > str.size()) { return false; }
    return str.compare(0, start.size(), start) == 0;
}

bool OPString::endsWith(const std::string& str, const std::string& end)
{
    if (end.size() > str.size()) { return false; }
    return str.compare(str.size() - end.size(), end.size(), end) == 0;
}

std::vector<std::string> OPString::split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    boost::split(result, str, boost::is_any_of(std::string(1, delimiter)));
    
    return result;
}

bool OPString::isNumber(const std::string &str)
{
    double number;
    return boost::conversion::try_lexical_convert<double>(str, number);
}

bool OPString::isInteger(const std::string &str)
{
    int number;
    return boost::conversion::try_lexical_convert<int>(str, number);
}

int OPString::getInt(const std::string &str)
{
    int number;
    
    if (!boost::conversion::try_lexical_convert<int>(str, number)) {
        return 0;
    }
    
    return number;
}

float OPString::getFloat(const std::string &str)
{
    float number;
    
    if (!boost::conversion::try_lexical_convert<float>(str, number)) {
        return 0;
    }
    
    return number;
}

bool OPString::contains(const std::string& str, const std::string& subStr)
{
    return boost::algorithm::contains(str, subStr);
}

std::string OPString::toString(const void* obj)
{
    const void * address = static_cast<const void*>(obj);
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();
    
    return name;
}

bool OPString::hasNonLatinCharacter(const std::string& text)
{
    std::u32string utf32String;
    if (!cocos2d::StringUtils::UTF8ToUTF32(text, utf32String)) {
        return true;
    } else {
        for (const auto& character : utf32String) {
            if (character > 0x02AF) {
                return true;
            }
        }
    }
    return false;
}
