//
//  OPLocalization.cpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 07/06/19.
//

#include "OPLocalization.hpp"

#include <json/document.h>
#include <json/rapidjson.h>

#include "../Debug/OPDebug.hpp"
#include "../Helpers/OPString.hpp"
#include "../Json/OPJson.hpp"

USING_NS_CC;
using namespace fairygui;

OPLocalization *OPLocalization::shared()
{
    static auto instance = new OPLocalization();
    return instance;
}

fairygui::GObject* OPLocalization::locateChild(fairygui::GComponent* root, const std::string& childPath)
{
    if (!root) {
        return nullptr;
    }
    
    return root->getChildByPath(childPath);
}

void OPLocalization::localizeTextField(fairygui::GComponent* root, const std::string& childPath, const std::string& key, const std::function<std::string(const std::string&)>& transform)
{
    auto textField = locateChild(root, childPath)->as<GTextField>();
    if (textField) {
        auto value = transform != nullptr ? transform(get(key)) : get(key);
        textField->setText(value);
    }
}

void OPLocalization::localizeInputPlaceholder(fairygui::GComponent* root, const std::string& childPath, const std::string& key)
{
    auto input = locateChild(root, childPath)->as<GTextInput>();
    if (input) {
        input->setPrompt(get(key));
    }
}

std::string OPLocalization::get(const std::string& key)
{
    auto instance = shared();
    
    auto result = instance->dictionary.find(key);
    if (result == instance->dictionary.end()) {
        return key;
    }
    
    return result->second;
}

std::string OPLocalization::getUppercased(const std::string& key)
{
    return OPString::uppercased(get(key));
}

std::string OPLocalization::get(const std::string& key, const char* arg...)
{
    auto text = get(key);

    va_list arguments;
    int argumentNumber = 0;
    std::string::size_type cindex;
    std::string newText = text;

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

std::string OPLocalization::get(const std::string &key, const std::vector<std::string> &args)
{
    auto text = get(key);

    int argumentNumber = 0;
    std::string::size_type cindex;
    std::string newText = text;

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

bool OPLocalization::initialize()
{
    auto fileUtils = FileUtils::getInstance();
    auto configPath = "config/localization.json";

    _deviceLanguage = Application::getInstance()->getCurrentLanguageCode();
    OPDebug::log("OPLocalization", "Device language = " + std::string(_deviceLanguage));

    if (!fileUtils->isFileExist(configPath))
    {
        OPDebug::error("OPLocalization", "Initialization failed because localization.json configuration file was not found.");
        return false;
    }
    
    rapidjson::Document document;
    if (!OPJson::parseFile(configPath, document))
    {
        OPDebug::error("OPLocalization", "Error while parsing config.json.");
        return false;
    }
    
    const char* testLanguage = document.HasMember("testLanguage") ? document["testLanguage"].GetString() : 0;
    auto defaultLanguage = document["defaultLanguage"].GetString();
    auto supportedLanguages = document["supportedLanguages"].GetArray();
    
    if (supportedLanguages.Size() == 0)
    {
        OPDebug::error("OPLocalization", "Key supportedLanguages is required in JSON.");
        return false;
    }
    
    if (std::find(supportedLanguages.begin(), supportedLanguages.end(), defaultLanguage) == supportedLanguages.end())
    {
        OPDebug::error("OPLocalization", "Key defaultLanguage is required and must be one of supportedLanguages.");
        return false;
    }

    if (testLanguage && std::find(supportedLanguages.begin(), supportedLanguages.end(), testLanguage) == supportedLanguages.end())
    {
        OPDebug::error("OPLocalization", "Key testLanguage must be one of supportedLanguages.");
        return false;
    }
    
    for (auto it = supportedLanguages.begin(); it != supportedLanguages.end(); ++it) {
        this->supportedLanguages.push_back(it->GetString());
    }
    
    this->defaultLanguage = defaultLanguage;

#if COCOS2D_DEBUG
    std::string language = testLanguage ? testLanguage : _deviceLanguage;
    return loadLanguage(language);
#else
    return loadLanguage(_deviceLanguage);
#endif
}

bool OPLocalization::loadLanguage(const std::string& languageCode)
{
    if (std::find(supportedLanguages.begin(), supportedLanguages.end(), languageCode) != supportedLanguages.end()) {
        currentLanguage = languageCode;
    }
    else
    {
        OPDebug::warn("OPLocalization", "Language " + languageCode + " was not found. Loading " + defaultLanguage + " instead.");
        currentLanguage = defaultLanguage;
    }
    
    auto path = "localization/" + currentLanguage + ".json";
    
    rapidjson::Document document;
    if (!OPJson::parseFile(path, document))
    {
        OPDebug::error("OPLocalization", "Could no load file: " + path);
        return false;
    }
    
    dictionary.clear();
    for (auto member = document.MemberBegin(); member != document.MemberEnd(); member++)
    {
        auto key = member->name.GetString();
        dictionary[key] = std::string(member->value.GetString());
    }
    

    OPDebug::log("OPLocalization", std::to_string(dictionary.size()) + " keys were loaded for language " + currentLanguage);
    
    return true;
}

const std::vector<std::string>& OPLocalization::getSupportedLanguages()
{
    return supportedLanguages;
}

const std::string& OPLocalization::getCurrentLanguage()
{
    return currentLanguage;
}

const std::string& OPLocalization::getDeviceLanguage() const
{
    return _deviceLanguage;
}
