//
//  OPSoundEffectsConfig.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 04/03/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPSoundEffectsConfig.hpp"
#include "../Json/OPJson.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Misc/OPPath.hpp"
#include <json/document-wrapper.h>

USING_NS_CC;
//
//cocos2d::Map<std::string, OPInstrumentItem*> OPSoundEffectsConfig::parseJson(const rapidjson::Value& json)
//{
//    OPDebug::ensure(OPJson::hasArray(json, "files"), "Member files not found.");
//    OPDebug::ensure(OPJson::hasObject(json, "aliases"), "Member aliases not found.");
//
//    cocos2d::Map<std::string, OPInstrumentItem*> result;
//    
//    auto files = OPJson::getStringVector(json, "files");
//    for (auto file : files)
//    {
//        auto id = OPPath::fileNameWithoutExtension(file);
//        auto item = OPInstrumentItem::create(std::to_string(result.size() + 1), "sfx/" + file);
//        result.insert(id, item);
//    }
//    
//    const auto& aliases = json["aliases"];
//    for (auto it = aliases.MemberBegin(); it != aliases.MemberEnd(); ++it) 
//    {
//        auto id = std::string(it->name.GetString());
//        auto item = OPInstrumentItem::create(std::to_string(result.size() + 1), "sfx/" + OPJson::getString(it->value, "file"));
//        item->setLoopEnabled(OPJson::getBool(it->value, "loop"));
//        result.insert(id, item);
//    }
//    
//    for (auto& pair : result)
//    {
//        auto item = pair.second;
//        item->setFilePath(FileUtils::getInstance()->fullPathForFilename(item->getFilePath()));
//    }
//
//    return result;
//}
