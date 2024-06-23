//
//  OPUserData.cpp
//  Opalib
//
//  Created by Adilson Tavares on 17/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPUserData.hpp"
#include "OPUserDataItem.hpp"
#include "../Json/OPJson.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Misc/OPFile.hpp"
#include "../Misc/OPCrypto.hpp"
#include <json/stringbuffer.h>
#include <json/prettywriter.h>
#include <cocos2d.h>

USING_NS_CC;

OPUserData* OPUserData::shared()
{
    static OPUserData* instance = 0;
    
    if (!instance) {
        instance = new OPUserData();
    }
    
    return instance;
}

void OPUserData::initialize()
{
    restore();
}

void OPUserData::restore()
{
    OPDebug::verb("OPUserData", "Restore from file.");

    auto encoded = OPFile::readString(getSavePath());

    std::string json;
    if (!OPCrypto::decrypt(encoded, json)){
        document = OPJson::document();
        OPDebug::log("OPUserData", "Not restored because no file with saved data was found.");
        return;
    }
    restore(json);
}

void OPUserData::restore(const std::string &json) {
    if (json.empty() || !OPJson::parse(json, document))
    {
        document = OPJson::document();
        OPDebug::log("OPUserData", "Not restored because no file with saved data was found.");
        return;
    }

    items.clear();

    for (auto it = document.MemberBegin(); it != document.MemberEnd(); ++it)
    {
        auto obj = it->value.GetObject();
        auto item = OPUserDataItem(obj);
        auto name = std::string(it->name.GetString(), it->name.GetStringLength());
        items[name] = item;
    }

    OPDebug::log("OPUserData", std::to_string(document.MemberCount()) + " keys restored.");
}

std::string OPUserData::getSavePath()
{
    return FileUtils::getInstance()->getWritablePath() + "user_data.dat";
}

void OPUserData::save()
{
    OPDebug::verb("OPUserData", "Save to file.");

    auto json = getContentAsJson();

    std::string encoded;
    OPCrypto::encrypt(json, encoded);
    
    OPFile::writeString(encoded, getSavePath());
}

void OPUserData::clear()
{
    items.clear();
}

bool OPUserData::getBool(const std::string &name, bool defaultValue)
{
    auto it = items.find(name);
    if (it == items.end()) {
        return defaultValue;
    }
    
    return items[name].value.boolValue;
}

bool OPUserData::hasKey(const std::string& name)
{
    auto it = items.find(name);
    return it != items.end();
}

void OPUserData::remove(const std::string& name)
{
    OPDebug::verb("OPUserData", "Remove " + name);

    auto it = items.find(name);
    if (it == items.end()) {
        return;
    }
    
    items.erase(it);
}

int OPUserData::getInt(const std::string &name, int defaultValue)
{
    auto it = items.find(name);
    if (it == items.end()) {
        return defaultValue;
    }
    
    return items[name].value.intValue;
}

float OPUserData::getFloat(const std::string &name, float defaultValue)
{
    auto it = items.find(name);
    if (it == items.end()) {
        return defaultValue;
    }
    
    return items[name].value.floatValue;
}

int64_t OPUserData::getInt64(const std::string &name, int64_t defaultValue)
{
    auto it = items.find(name);
    if (it == items.end()) {
        return defaultValue;
    }
    
    return items[name].value.int64Value;
}

std::string OPUserData::getString(const std::string &name, const std::string& defaultValue)
{
    auto it = items.find(name);
    if (it == items.end()) {
        return defaultValue;
    }
    
    return items[name].value.stringValue;
}

void OPUserData::onSetValue(bool autoSave)
{
    if (autoSave) {
        save();
    }
}

void OPUserData::setBool(const std::string& name, bool value, bool autoSave)
{
    OPDebug::verb("OPUserData", "setBool " + name + " = " + std::to_string(value));

    items[name] = OPUserDataItem(value);
    onSetValue(autoSave);
}

void OPUserData::setInt(const std::string &name, int value, bool autoSave)
{
    OPDebug::verb("OPUserData", "setInt " + name + " = " + std::to_string(value));

    items[name] = OPUserDataItem(value);
    onSetValue(autoSave);
}

void OPUserData::setInt64(const std::string &name, int64_t value, bool autoSave)
{
    OPDebug::verb("OPUserData", "setInt64 " + name + " = " + std::to_string(value));
    
    items[name] = OPUserDataItem(value);
    onSetValue(autoSave);
}

void OPUserData::setFloat(const std::string &name, float value, bool autoSave)
{
    OPDebug::verb("OPUserData", "setFloat " + name + " = " + std::to_string(value));

    items[name] = OPUserDataItem(value);
    onSetValue(autoSave);
}

void OPUserData::setString(const std::string &name, const std::string &value, bool autoSave)
{
    OPDebug::verb("OPUserData", "setString " + name + " = " + value);

    items[name] = OPUserDataItem(value);
    onSetValue(autoSave);
}

void OPUserData::printItemsForDebug()
{
    OPDebug::log("OPUserData", "Current state (" + std::to_string(items.size()) + " keys):");
    
    for (auto item : items) {
        OPDebug::log("OPUserData", item.first + " = " + item.second.valueAsString());
    }
}

OPUserData::~OPUserData()
{
    save();
    OPDebug::log("OPUserData", "Release");
}

std::string OPUserData::getContentAsJson() {
    auto document = OPJson::document();

    for (auto item : items)
    {
        rapidjson::Value value = item.second.toJson(document);
        OPJson::set(document, item.first, value);
    }

    return OPJson::toString(document);
}

