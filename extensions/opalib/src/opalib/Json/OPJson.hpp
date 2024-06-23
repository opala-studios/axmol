//
//  OPJson.hpp
//  RapidJsonLib
//
//  Created by Adilson Tavares on 23/07/19.
//  Copyright © 2019 Adilson Tavares. All rights reserved.
//

#ifndef OPJson_hpp
#define OPJson_hpp

#include <iostream>
#include <cocos2d.h>
#include <json/document-wrapper.h>

class OPJson
{
public:
    
    const static rapidjson::Value kEmptyArray;
    const static rapidjson::Value kEmptyObject;
    
private:
    
    template<typename T, typename TGetResult, bool (rapidjson::Value::*TCheck)() const, TGetResult(rapidjson::Value::*TGet)() const>
    static std::vector<T> getVector(const rapidjson::Value& object, const char* key)
    {
        std::vector<T> items;

        if (!object.HasMember(key)) { return items; }
        
        const auto& member = object[key];
        if (!member.IsArray()) { return items; }

        for (auto it = member.Begin(); it != member.End(); ++it)
        {
            if (((*it).*TCheck)()) {
                items.push_back(((*it).*TGet)());
            }
        }
        
        return items;
    }
    
//    template<typename T>
//    static void setArray(rapidjson::Document& document,
//                         rapidjson::Value& object,
//                         const std::string& key,
//                         T&& data, const int size)
//    {
//        auto array = OPJson::array();
//        
//        for (int i = 0; i < size; i++)
//        {
//            array.PushBack(data[i], document.GetAllocator());
//        }
//        
//        OPJson::set(document, object, key, array);
//    }
    
public:
    
    static rapidjson::Document document();
    static rapidjson::Value array();
    static rapidjson::Value object();

    static bool getBool(const rapidjson::Value& object, const char* key, bool defaultValue = false);
    static int getInt(const rapidjson::Value& object, const char* key, int defaultValue = 0);
    static float getFloat(const rapidjson::Value& object, const char* key, float defaultValue = 0);
    static int64_t getLong(const rapidjson::Value& object, const char* key, int64_t defaultValue = 0);
    static double getDouble(const rapidjson::Value& object, const char* key, double defaultValue = 0);
    static std::string getString(const rapidjson::Value& object, const char* key, const std::string& defaultValue = "");
    static const char* getStringConst(const rapidjson::Value& object, const char* key, const char* defaultValue = 0);
    static cocos2d::Size getSize(const rapidjson::Value& object, const char* key, const cocos2d::Size& defaultValue = cocos2d::Size::ZERO);
    
    static std::vector<std::string> getStringVector(const rapidjson::Value& object, const char* key);
    static std::vector<int> getIntVector(const rapidjson::Value& object, const char* key);
    static std::vector<float> getFloatVector(const rapidjson::Value& object, const char* key);
    
    static void set(rapidjson::Document& document, const std::string& key, bool boolValue);
    static void set(rapidjson::Document& document, const std::string& key, int intValue);
    static void set(rapidjson::Document& document, const std::string& key, float floatValue);
    static void set(rapidjson::Document& document, const std::string& key, int64_t longValue);
    static void set(rapidjson::Document& document, const std::string& key, double doubleValue);
    static void set(rapidjson::Document& document, const std::string& key, const std::string& stringValue);
    static void set(rapidjson::Document& document, const std::string& key, const char* stringValue);
    static void set(rapidjson::Document& document, const std::string& key, const char* stringValue, std::size_t size);
    static void set(rapidjson::Document& document, const std::string& key, rapidjson::Value& value);
    
    template<typename T>
    static void set(rapidjson::Document& document,
                    rapidjson::Value& object,
                    const std::string& key,
                    const std::vector<T>& list)
    {
        auto array = OPJson::array();
        
        for (auto item : list)
            OPJson::append(document, array, item);
        
        OPJson::set(document, object, key, array);
    }
    
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, bool boolValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int intValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, float floatValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, int64_t int64Value);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, double doubleValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const std::string& stringValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, const char* stringValue, std::size_t size);
    static void set(rapidjson::Document& document, rapidjson::Value& object, const std::string& key, rapidjson::Value& value);
    
    static void append(rapidjson::Document& document, bool boolValue);
    static void append(rapidjson::Document& document, int intValue);
    static void append(rapidjson::Document& document, float floatValue);
    static void append(rapidjson::Document& document, int64_t int64Value);
    static void append(rapidjson::Document& document, double doubleValue);
    static void append(rapidjson::Document& document, const std::string& stringValue);
    static void append(rapidjson::Document& document, const char* stringValue);
    static void append(rapidjson::Document& document, const char* stringValue, std::size_t size);
    static void append(rapidjson::Document& document, rapidjson::Value& value);
    
    static void append(rapidjson::Document& document, rapidjson::Value& object, bool boolValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, int intValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, float floatValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, int64_t int64Value);
    static void append(rapidjson::Document& document, rapidjson::Value& object, double doubleValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, const std::string& stringValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue);
    static void append(rapidjson::Document& document, rapidjson::Value& object, const char* stringValue, std::size_t size);
    static void append(rapidjson::Document& document, rapidjson::Value& object, rapidjson::Value& value);
    
    static bool hasBool(const rapidjson::Value& object, const std::string& memberName);
    static bool hasNumber(const rapidjson::Value& object, const std::string& memberName);
    static bool hasString(const rapidjson::Value& object, const std::string& memberName);
    static bool hasObject(const rapidjson::Value& object, const std::string& memberName);
    static bool hasArray(const rapidjson::Value& object, const std::string& memberName);
    
    static bool exists(const rapidjson::Value& value, const std::string& memberName);
    static bool existsPath(rapidjson::Value& value, const std::string& path);
    
    static bool parse(const std::string& json, rapidjson::Document& document);
    static bool parseFile(const std::string& filePath, rapidjson::Document& document);
    
    static std::string toString(const rapidjson::Value& document, bool pretty = false);
    static void save(const rapidjson::Document& document, const std::string& file);
};

#endif /* OPJson_hpp */
