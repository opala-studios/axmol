//
//  OPLocalization.hpp
//  DrumsCocos
//
//  Created by Adilson Tavares on 07/06/19.
//

#ifndef OPLocalization_hpp
#define OPLocalization_hpp

#include <cocos2d.h>
#include <FairyGUI.h>

struct TranslatableItem {
    std::string childPath;
    std::string localizationKey;
    bool uppercased;
    std::vector<std::string> args;
    std::string startText;
};

class OPLocalization
{
private:
    
    std::string defaultLanguage;
    std::string currentLanguage;
    std::string _deviceLanguage;
    
    std::vector<std::string> supportedLanguages;
    std::map<std::string, std::string> dictionary;
    
    static fairygui::GObject* locateChild(fairygui::GComponent* root, const std::string& childPath);
    
public:
    
    bool initialize();
    
    static OPLocalization* shared();

    static std::string getUppercased(const std::string& key);

    static std::string get(const std::string& key);
    static std::string get(const std::string& key, const char* args...);
    static std::string get(const std::string& key, const std::vector<std::string>& args);
    
    static void localizeTextField(fairygui::GComponent* root, const std::string& childPath, const std::string& key, const std::function<std::string(const std::string&)>& transform = nullptr);
    static void localizeInputPlaceholder(fairygui::GComponent* root, const std::string& childPath, const std::string& key);

    const std::string& getDeviceLanguage() const;
    const std::string& getCurrentLanguage();
    const std::vector<std::string>& getSupportedLanguages();
    
    bool loadLanguage(const std::string& languageCode);
};

#endif /* OPLocalization_hpp */
