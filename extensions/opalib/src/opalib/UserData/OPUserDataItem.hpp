//
//  OPUserDataItem.hpp
//  Opalib
//
//  Created by Adilson Tavares on 17/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPUserDataItem_hpp
#define OPUserDataItem_hpp

#include <iostream>
#include <json/document-wrapper.h>

struct OPUserDataItem
{
    union Value
    {
        int intValue;
        bool boolValue;
        float floatValue;
        char* stringValue;
        int64_t int64Value;
    };
    
    enum Type
    {
        BOOL,
        INT,
        FLOAT,
        INT64,
        STRING,
    };
    
    Type type;
    Value value;
    
    std::string valueAsString();
    
    OPUserDataItem();
    OPUserDataItem(const rapidjson::Value& root);
    OPUserDataItem(bool value);
    OPUserDataItem(int value);
    OPUserDataItem(float value);
    OPUserDataItem(int64_t value);
    OPUserDataItem(const std::string& value);
    
    rapidjson::Value toJson(rapidjson::Document& document);
    
    ~OPUserDataItem();
};

#endif /* OPUserDataItem_hpp */
