//
//  OPAudio.cpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 05/08/19.
//

#include <external/json/document.h>
#include "OPSoundEffects.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Misc/OPPath.hpp"
#include "../Misc/OPDirectory.hpp"
#include "../Json/OPJson.hpp"
#include "./OPSoundEffectsConfig.hpp"

USING_NS_CC;

OPSoundEffects::OPSoundEffects()
{
}

OPSoundEffects* OPSoundEffects::shared()
{
    static OPSoundEffects* instance = new OPSoundEffects();
    return instance;
}

void OPSoundEffects::initialize()
{
    if (initialized) {
        return;
    }

//    OPDebug::log("SoundEffects", "Initialize");
//
//    auto configPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("config/sfx.json");
//
//    rapidjson::Document json;
//    OPDebug::ensure(OPJson::parseFile(configPath, json), "Could not parse OPSoundEffects config file.");
//
//    items = OPSoundEffectsConfig::parseJson(json);
//
//    Vector<OPInstrumentItem*> instrumentItems;
//    for (auto item : items) {
//        instrumentItems.pushBack(item.second);
//    }
//
//    instrument = OPInstrument::create(instrumentItems);
//    OPDebug::ensure(instrument, "Could not create instrument.");
//    instrument->retain();
//    instrument->loadAsync(0);
//
//    initialized = true;

    OPDebug::log("SoundEffects", "Initialization success");
}

bool OPSoundEffects::play(const std::string &name)
{
//    auto item = items.at(name);
//    if (!item)
//    {
//        OPDebug::log("OPAudio", "Could not find audio for name " + name);
//        return false;
//    }
    
//    instrument->play(item->getId());
    
    return true;
}

bool OPSoundEffects::stop(const std::string& name)
{
//    auto item = items.at(name);
//    if (!item)
//    {
//        OPDebug::log("OPAudio", "Could not stop audio for name " + name);
//        return false;
//    }
    
//    instrument->stop(item->getId());
    
    return true;
}

OPSoundEffects::~OPSoundEffects()
{
//    CC_SAFE_RELEASE_NULL(instrument);
}
