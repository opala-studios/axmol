//
//  OPJson.cpp
//  RapidJsonLib
//
//  Created by Adilson Tavares on 23/07/19.
//  Copyright © 2019 Adilson Tavares. All rights reserved.
//

#include "OPJson.hpp"
#include "../Debug/OPDebug.hpp"

#include "../Helpers/OPString.hpp"

#include <fstream>
#include <json/document.h>
#include <json/prettywriter.h>
#include <json/writer.h>
#include <json/stringbuffer.h>
#include <json/filereadstream.h>
#include <cocos2d.h>

USING_NS_CC;

const rapidjson::Value OPJson::kEmptyArray = rapidjson::Value(rapidjson::kArrayType);
const rapidjson::Value OPJson::kEmptyObject = rapidjson::Value(rapidjson::kObjectType);

rapidjson::Document OPJson::document()
{
    auto document = rapidjson::Document();
    document.SetObject();
    
    return document;
}

rapidjson::Value OPJson::array()
{
    return rapidjson::Value(rapidjson::kArrayType);
}

rapidjson::Value OPJson::object()
{
    return rapidjson::Value(rapidjson::kObjectType);
}

bool OPJson::getBool(const rapidjson::Value& object, const char* key, bool defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsBool()) {
        return defaultValue;
    }
    
    return object[key].GetBool();
}

int OPJson::getInt(const rapidjson::Value& object, const char* key, int defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsInt()) {
        return defaultValue;
    }
    
    return object[key].GetInt();
}

float OPJson::getFloat(const rapidjson::Value& object, const char* key, float defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsNumber()) {
        return defaultValue;
    }
    
    return object[key].GetFloat();
}

int64_t OPJson::getLong(const rapidjson::Value& object, const char* key, int64_t defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsInt64()) {
        return defaultValue;
    }
    
    return object[key].GetInt64();
}

double OPJson::getDouble(const rapidjson::Value& object, const char* key, double defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsNumber()) {
        return defaultValue;
    }
    
    return object[key].GetDouble();
}

std::string OPJson::getString(const rapidjson::Value& object, const char* key, const std::string& defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsString()) {
        return defaultValue;
    }
    
    return object[key].GetString();
}

const char* OPJson::getStringConst(const rapidjson::Value& object, const char* key, const char* defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsString()) {
        return defaultValue;
    }
    
    return object[key].GetString();
}

cocos2d::Size OPJson::getSize(const rapidjson::Value& object, const char* key, const cocos2d::Size& defaultValue)
{
    if (!object.HasMember(key) || !object[key].IsString()) {
        return defaultValue;
    }
    
    float width, height;
    sscanf(object[key].GetString(), "%f;%f", &width, &height);
    
    return cocos2d::Size(width, height);
}

std::vector<std::string> OPJson::getStringVector(const rapidjson::Value& object, const char* key)
{
    return getVector<std::string, const char*, &rapidjson::Value::IsString, &rapidjson::Value::GetString>(object, key);
}

std::vector<int> OPJson::getIntVector(const rapidjson::Value& object, const char* key)
{
    return getVector<int, int, &rapidjson::Value::IsInt, &rapidjson::Value::GetInt>(object, key);
}

std::vector<float> OPJson::getFloatVector(const rapidjson::Value& object, const char* key)
{
    return getVector<float, float, &rapidjson::Value::IsNumber, &rapidjson::Value::GetFloat>(object, key);
}

bool OPJson::parse(const std::string& json, rapidjson::Document& document)
{
    document.Parse(json.c_str(), json.length());
    
    if (document.HasParseError()) {
        return false;
    }
    
    return true;
}

bool OPJson::parseFile(const std::string& filePath, rapidjson::Document& document)
{
    auto contents = FileUtils::getInstance()->getStringFromFile(filePath);
    
    if (contents.size() == 0)
    {
        OPDebug::warn("Json", "Could not parse JSON file. It is empty or was not found: " + filePath);
        return 0;
    }
    
    return parse(contents, document);
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, bool boolValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, boolValue, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int intValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, intValue, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, float floatValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, floatValue, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int64_t int64Value)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, (int64_t)int64Value, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, double doubleValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, doubleValue, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const std::string& stringValue)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    rapidjson::Value value(stringValue.c_str(), (unsigned int)stringValue.size(), document.GetAllocator());
    
    object.AddMember(name, value, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue)
{
    set(document, object, key, stringValue, strlen(stringValue));
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue, std::size_t size)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    rapidjson::Value value(stringValue, (unsigned int)size, document.GetAllocator());
    
    object.AddMember(name, value, document.GetAllocator());
}

void OPJson::set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, rapidjson::Value& value)
{
    rapidjson::Value name(key.c_str(), (unsigned int)key.size(), document.GetAllocator());
    object.AddMember(name, value, document.GetAllocator());
}

void OPJson::set(rapidjson::Document &document, const std::string &key, rapidjson::Value &value)
{
    set(document, document, key, value);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, bool boolValue)
{
    set(document, document, key, boolValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, int intValue)
{
    set(document, document, key, intValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, float floatValue)
{
    set(document, document, key, floatValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, int64_t int64Value)
{
    set(document, document, key, int64Value);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, double doubleValue)
{
    set(document, document, key, doubleValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, const std::string& stringValue)
{
    set(document, document, key, stringValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, const char* stringValue)
{
    set(document, document, key, stringValue);
}

void OPJson::set(rapidjson::Document& document, const std::string& key, const char* stringValue, std::size_t size)
{
    set(document, document, key, stringValue, size);
}

std::string OPJson::toString(const rapidjson::Value& value, bool pretty)
{
    rapidjson::StringBuffer buffer;
    
    if (pretty)
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        value.Accept(writer);
    }
    else
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        value.Accept(writer);
    }
    
    return std::string(buffer.GetString(), buffer.GetSize());
}


void OPJson::append(rapidjson::Document& document, bool boolValue)
{
    append(document, document, boolValue);
}

void OPJson::append(rapidjson::Document& document, int intValue)
{
    append(document, document, intValue);
}

void OPJson::append(rapidjson::Document& document, float floatValue)
{
    append(document, document, floatValue);
}

void OPJson::append(rapidjson::Document& document, int64_t int64Value)
{
    append(document, document, int64Value);
}

void OPJson::append(rapidjson::Document& document, double doubleValue)
{
    append(document, document, doubleValue);
}

void OPJson::append(rapidjson::Document& document, const std::string& stringValue)
{
    append(document, document, stringValue);
}

void OPJson::append(rapidjson::Document& document, const char* stringValue)
{
    append(document, document, stringValue);
}

void OPJson::append(rapidjson::Document& document, const char* stringValue, std::size_t size)
{
    append(document, document, stringValue, size);
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& value)
{
    append(document, document, value);
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, bool boolValue)
{
    object.PushBack(boolValue, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, int intValue)
{
    object.PushBack(intValue, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, float floatValue)
{
    object.PushBack(floatValue, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, int64_t int64Value)
{
    object.PushBack((int64_t)int64Value, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, double doubleValue)
{
    object.PushBack(doubleValue, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, const std::string& stringValue)
{
    rapidjson::Value value(stringValue.c_str(), (unsigned int)stringValue.size(), document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue)
{
    rapidjson::Value value(stringValue, (unsigned int)strlen(stringValue), document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue, std::size_t size)
{
    rapidjson::Value value(stringValue, (unsigned int)size, document.GetAllocator());
    object.PushBack(value, document.GetAllocator());
}

void OPJson::append(rapidjson::Document& document, rapidjson::Value& object, rapidjson::Value& value)
{
    object.PushBack(value, document.GetAllocator());
}

bool OPJson::exists(const rapidjson::Value& value, const std::string& memberName)
{
    return value.IsObject() && value.HasMember(memberName.c_str());
}

bool OPJson::existsPath(rapidjson::Value &value, const std::string &path)
{
    rapidjson::Value* temp = &value;
    
    auto components = OPString::split(path, '/');
    
    for (auto comp : components)
    {
        if (comp.size() != 0)
        {
            if (temp->IsObject() && temp->HasMember(comp.c_str())) {
                temp = &(*temp)[comp.c_str()];
            }
            else if (temp->IsArray() && OPString::isNumber(comp))
            {
                int index = OPString::getInt(comp);
                
                if (index < 0 || index >= temp->Size()) {
                    return false;
                }
                
                temp = &(*temp)[index];
            }
            else {
                return false;
            }
        }
    }
    
    return true;
}

void OPJson::save(const rapidjson::Document& document, const std::string& file)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    auto jsonStr = buffer.GetString();
    FileUtils::getInstance()->writeStringToFile(jsonStr, file);
}

bool OPJson::hasBool(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsBool();
}

bool OPJson::hasNumber(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsNumber();
}

bool OPJson::hasString(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsString();
}

bool OPJson::hasObject(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsObject() || member.IsNull();
}

bool OPJson::hasArray(const rapidjson::Value& object, const std::string& memberName)
{
    if (!exists(object, memberName)) { return false; }
    const auto& member = object[memberName.c_str()];
    return member.IsArray() || member.IsNull();
}

