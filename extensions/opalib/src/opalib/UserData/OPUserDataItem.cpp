//
//  OPUserDataItem.cpp
//  Opalib
//
//  Created by Adilson Tavares on 17/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPUserDataItem.hpp"
#include "../Json/OPJson.hpp"
#include "../Debug/OPDebug.hpp"

OPUserDataItem::OPUserDataItem()
{
    type = OPUserDataItem::Type::BOOL;
    value.boolValue = false;
}

OPUserDataItem::OPUserDataItem(const rapidjson::Value& root)
{
    type = (OPUserDataItem::Type)root["type"].GetInt();
    
    switch (type)
    {
        case OPUserDataItem::Type::BOOL:
            value.boolValue = root["value"].GetBool();
            break;
            
        case OPUserDataItem::Type::INT:
            value.intValue = root["value"].GetInt();
            break;
            
        case OPUserDataItem::Type::FLOAT:
            value.floatValue = root["value"].GetFloat();
            break;
        
        case OPUserDataItem::Type::INT64:
            value.int64Value = root["value"].GetInt64();
            break;
            
        case OPUserDataItem::Type::STRING:
        {
            auto& val = root["value"];   
            value.stringValue = new char[val.GetStringLength() + 1];
            strcpy(value.stringValue, val.GetString());
            value.stringValue[val.GetStringLength()] = '\0';
            break;
        }
    }
}

rapidjson::Value OPUserDataItem::toJson(rapidjson::Document& document)
{
    rapidjson::Value item = OPJson::object();
    
    OPJson::set(document, item, "type", (int)type);
    
    switch (type)
    {
        case Type::BOOL:
            OPJson::set(document, item, "value", value.boolValue);
            break;
            
        case Type::INT:
            OPJson::set(document, item, "value", value.intValue);
            break;
            
        case Type::FLOAT:
            OPJson::set(document, item, "value", value.floatValue);
            break;
            
        case Type::INT64:
            OPJson::set(document, item, "value", value.int64Value);
            break;

        case Type::STRING:
            OPJson::set(document, item, "value", value.stringValue);
            break;
    }
    
    return item;
}

std::string OPUserDataItem::valueAsString()
{
    switch (type)
    {
        case OPUserDataItem::Type::BOOL:
            return std::to_string(value.boolValue);
            
        case OPUserDataItem::Type::INT:
            return std::to_string(value.intValue);
            
        case OPUserDataItem::Type::FLOAT:
            return std::to_string(value.floatValue);

        case OPUserDataItem::Type::INT64:
            return std::to_string(value.int64Value);
            
        case OPUserDataItem::Type::STRING:
            return value.stringValue;
            
        default:
            return "";
    }
}

OPUserDataItem::OPUserDataItem(bool boolValue)
{
    type = OPUserDataItem::Type::BOOL;
    value.boolValue = boolValue;
}

OPUserDataItem::OPUserDataItem(int intValue)
{
    type = OPUserDataItem::Type::INT;
    value.intValue = intValue;
}

OPUserDataItem::OPUserDataItem(float floatValue)
{
    type = OPUserDataItem::Type::FLOAT;
    value.floatValue = floatValue;
}

OPUserDataItem::OPUserDataItem(int64_t int64Value)
{
    type = OPUserDataItem::Type::INT64;
    value.int64Value = int64Value;
}

OPUserDataItem::OPUserDataItem(const std::string& stringValue)
{
    type = OPUserDataItem::Type::STRING;
    
    value.stringValue = new char[stringValue.size() + 1];
    strcpy(value.stringValue, stringValue.c_str());
}

OPUserDataItem::~OPUserDataItem()
{
}
