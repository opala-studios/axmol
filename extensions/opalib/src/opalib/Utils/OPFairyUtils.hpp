//
//  FairyUtils.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 10/08/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPFairyUtils_hpp
#define OPFairyUtils_hpp

#include <FairyGUI.h>
#include <exception>

class OPFairyUtils {
public:

    static void positionOnSafeArea(fairygui::GObject* object, bool hasBanner);
    static fairygui::GComponent* createMainView(fairygui::GRoot* root, const std::string& packageName, const std::string& viewName, bool hasBanner);
    static fairygui::GComponent* createMainView(const std::string& packageName, const std::string& viewName, bool hasBanner = false);
    static void center(fairygui::GObject* object);
    static void addSingleTimeClickListener(
        fairygui::GObject* object,
        const fairygui::EventCallback& callback,
        const fairygui::EventTag& tag = fairygui::EventTag::None
    );
    static cocos2d::Vec2 getCenter(fairygui::GObject* object);
    template<typename T>
    static T* findChild(const fairygui::GComponent* component, const std::string& childPath) {
        auto found = component->getChild(childPath);
        if (found == nullptr) {
            std::stringstream str;
            str << "Unable to find a child of '" << component->getPackageItem()->name << "' on path '" << childPath << "'";
            throw std::runtime_error(str.str());
        }
        auto casted = found->as<T>();
        if (casted == nullptr) {
            std::stringstream str;
            str << "Found a child of '" << component->getPackageItem()->name << "' on path '" << childPath << "', but it isn't of type '";
            str << typeid(T).name() << "'.";
            throw std::runtime_error(str.str());
        }
        return casted;
    }
    template<typename T>
    static T* create(const std::string& pkgName, const std::string& resName) {
        auto created = fairygui::UIPackage::createObject(pkgName, resName);
        if (created == nullptr) {
            throw std::runtime_error("Unable to create specified object. (Is pkgName and resName correct? Is the published FairyGUI package updated?)");
        }
        T* casted = created->as<T>();
        if (casted == nullptr) {
            throw std::runtime_error("Created objected does not matched requested type.");
        }
        return casted;
    }

    template<typename T>
    static T* getGLoaderChild(fairygui::GComponent* component, const std::string& loaderPath, const std::string& childPath) {
        return component->getChildByPath(loaderPath)->template as<fairygui::GLoader>()->getComponent()->getChildByPath(childPath)->
            template as<T>();
    }
	
	static std::string getFullPath(fairygui::GObject* object);
	static cocos2d::Vec2 getScreenPosition(fairygui::GObject* object);
	static cocos2d::Vec2 getScreenFactor();
};

/**
 * @deprecated Use OPFairyUtils, the convention is to use OP for shared stuff.
 */
typedef OPFairyUtils FairyUtils;

#endif /* FairyUtils_hpp */
