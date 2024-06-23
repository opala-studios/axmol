//
// Created by Ricardo on 10/23/2020.
//

#include "OPSoundController.h"
#include <cocos2d.h>
#include <json/document-wrapper.h>
#include "opalib/Debug/OPDebug.hpp"
#include "opalib/Json/OPJson.hpp"
#include "opalib/Misc/OPPath.hpp"
#include "opalib/Crash/OPCrashAnalytics.hpp"

OPSoundController *OPSoundController::shared() {
    static OPSoundController* instance = new OPSoundController();
    return instance;
}

void OPSoundController::initialize() {
    auto configPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("config/sfx.json");
    _synthItems = std::move(load_json(configPath));
    _synthesizer = std::make_unique<OPSynthesizer>();

    for (auto& kv : _synthItems){
        _synthesizer->loadItem(kv.second.get(), nullptr);
    }
}

std::unordered_map<std::string, std::shared_ptr<OPSynthItem>> OPSoundController::load_json(const std::string &filepath) {
    rapidjson::Document json;
    OPDebug::ensure(OPJson::parseFile(filepath, json), "Could not parse OPSoundEffects config file.");
    OPDebug::ensure(OPJson::hasArray(json, "files"), "Member files not found.");
    OPDebug::ensure(OPJson::hasObject(json, "aliases"), "Member aliases not found.");

    std::unordered_map<std::string, std::shared_ptr<OPSynthItem>> result;

    auto files = OPJson::getStringVector(json, "files");
    for (auto& file : files) {
        auto name = OPPath::fileNameWithoutExtension(file);
        auto item = std::make_shared<OPSynthItem>("sfx/" + file, result.size() + 1);
        result.emplace(name, item);
    }

    const auto& aliases = json["aliases"];
    for (auto it = aliases.MemberBegin(); it != aliases.MemberEnd(); ++it) {
        auto name = std::string(it->name.GetString());
        auto item = std::make_shared<OPSynthItem>("sfx/" + OPJson::getString(it->value, "file"), result.size() + 1);
		auto loop = OPJson::getBool(it->value, "loop");
		item->setLoop(loop);
		
        result.emplace(name, item);
    }

    for (auto& pair : result) {
        auto item = pair.second;
        item->setFilePath(cocos2d::FileUtils::getInstance()->fullPathForFilename(item->getFilePath()));
    }
    return result;
}

void OPSoundController::play(const std::string &soundName) {
    auto synthItem = _synthItems.find(soundName);
    if (synthItem == _synthItems.end()){
        OP_LOG_ERROR("Could not find a SynthItem with the name " + soundName);
        return;
    }
    _synthesizer->play(synthItem->second.get());
}
void OPSoundController::playByPath(const std::string& soundName, const std::string &soundPath) {
    auto synthItem = _synthItems.find(soundName);
    if (synthItem == _synthItems.end()){
        addSound(soundName, soundPath, [this, soundName](bool success){
            if (success) {
                play(soundName);
            }
        });
    } else {
        _synthesizer->play(synthItem->second.get());
    }
}

void OPSoundController::stop(const std::string &soundName) {
    auto synthItem = _synthItems.find(soundName);
    if (synthItem == _synthItems.end()){
        OP_LOG_ERROR("Could not find a SynthItem with the name " + soundName);
        return;
    }
    _synthesizer->stop(synthItem->second.get());
}

void OPSoundController::stopAll()
{
    for (auto item : _synthItems)
    {
        _synthesizer->stop(item.second.get());
    }
}

void OPSoundController::addSound(const std::string &name, const std::string &filePath,
                                 const LoadSoundHandler &onCompletion) {
    if (_synthItems.find(name) != _synthItems.end()){
        OP_LOG("Sound " + name + " already loaded");
        if(onCompletion){
            onCompletion(true);
        }
        return;
    }

    auto item = std::make_shared<OPSynthItem>(filePath, _synthItems.size() + 1);
    _synthItems.emplace(name, item);
    _synthesizer->loadItem(item.get(), [this, name, onCompletion](bool success){

        if (!success){
            _synthItems.erase(name);
            OP_LOG_ERROR("Failed to load sound " + name);
        }

        if (onCompletion){
            onCompletion(success);
        }
    });
    
}
