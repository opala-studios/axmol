//
//  OPString.hpp
//  Opalib
//
//  Created by Adilson Tavares on 12/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPString_hpp
#define OPString_hpp

#include <iostream>

class OPString
{
public:
    
    static std::string uuid();

    static bool startsWith(const std::string& str, const std::string& start);
    static bool endsWith(const std::string& str, const std::string& end);
    
    static std::string uppercased(const std::string& str);
    static std::string lowercased(const std::string& str);
    
    static bool queryMatches(const std::string& query, const std::string& text);
    static std::string normalizeForQuery(const std::string& query);
    
    static std::string captureStringArguments(const std::string input, const char* arg...);
    static std::string captureStringArguments(const std::string input, const std::vector<std::string> &args);
    
    static void replace(std::string& string, const std::string& occurrence, const std::string& replacement);
    static std::string replacing(const std::string& string, const std::string& occurrence, const std::string& replacement);
    
    static void trim(std::string& string, const std::string& characters = " ");
    static std::string trimmed(const std::string& string, const std::string& characters = " ");
    
    static bool isNumber(const std::string& str);
    static bool isInteger(const std::string& str);
    
    static int getInt(const std::string& str);
    static float getFloat(const std::string& str);
    
    static bool contains(const std::string& str, const std::string& subStr);
    
    static std::string toString(const void*);
    
    static std::vector<std::string> split(const std::string& str, char delimiter);

    static bool hasNonLatinCharacter(const std::string& text);
};

#endif /* OPString_hpp */
