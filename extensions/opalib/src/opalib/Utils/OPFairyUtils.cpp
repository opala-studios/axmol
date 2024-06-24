//
//  FairyUtils.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 10/08/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPFairyUtils.hpp"
#include "../Misc/OPSafeArea.hpp"
//#include "../Ads/OPAds.hpp"

using namespace fairygui;

void OPFairyUtils::positionOnSafeArea(GObject* object, bool hasBanner)
{
    auto safeFairyTopPos = OPSafeArea::top();
    auto safeLeft = OPSafeArea::left();

    auto safeHeight = OPSafeArea::height();
    auto safeWidth = OPSafeArea::width();

    object->setY(safeFairyTopPos);
    object->setX(safeLeft);

    object->setSize(safeWidth, safeHeight);
}

fairygui::GComponent *OPFairyUtils::createMainView(fairygui::GRoot *root, const std::string &packageName, const std::string &viewName, bool hasBanner)
{
    UIPackage::addPackage("fairy/" + packageName);
    
    auto rootSize = root->getSize();
    
    auto view = UIPackage::createObject(packageName, viewName)->as<GComponent>();
    if (!view) {
        return nullptr;
    }
    
    view->setSize(rootSize.width, rootSize.height);
    UIRoot->addChild(view);
    
    auto safeArea = view->getChild("safeArea");
    if (safeArea) {
        positionOnSafeArea(safeArea, hasBanner);
    }
    
    return view;
}

fairygui::GComponent *OPFairyUtils::createMainView(const std::string &packageName, const std::string &viewName, bool hasBanner)
{
    return createMainView(UIRoot, packageName, viewName, hasBanner);
}

void OPFairyUtils::addSingleTimeClickListener(fairygui::GObject *object, const fairygui::EventCallback& callback, const fairygui::EventTag& tag) {
    object->addClickListener([object, callback, tag](EventContext* context){
        callback(context);
        object->removeClickListener(tag);
    }, tag);
}

cocos2d::Vec2 OPFairyUtils::getCenter(fairygui::GObject* object)
{
    auto size = object->getSize();
    return {object->getX() + size.width / 2, object->getY() + size.height / 2};
}

std::string OPFairyUtils::getFullPath(fairygui::GObject* object)
{
	if(object->findParent() == nullptr)
		return "view";
	
	std::string result = object->name;
	fairygui::GObject* parent = object->findParent();
	
	while (parent != nullptr) {
		if(parent->name != "") {
			if(result != "") {
				result = parent->name + "." + result;
			} else {
				result = parent->name;
			}
		}
		parent = parent->findParent();
	}
	
	return result;
}

cocos2d::Vec2 OPFairyUtils::getScreenPosition(fairygui::GObject* object)
{
	cocos2d::Vec2 result = object->getPosition();
	auto parent = object->findParent();
	
	while (parent != nullptr){
		result = parent->getPosition() + result;
		parent = parent->findParent();
	}
	
	return result;
}

cocos2d::Vec2 OPFairyUtils::getScreenFactor() {
    auto glScreenSize =
        cocos2d::Director::getInstance()->getGLView()->getFrameSize();
    auto sceneScreenSize =
        cocos2d::Director::getInstance()->getRunningScene()->getContentSize();

    cocos2d::Vec2 result;

    result.x = glScreenSize.width / sceneScreenSize.width;
    result.y = glScreenSize.height / sceneScreenSize.height;

    return result;
}

void OPFairyUtils::center(fairygui::GObject* object) {
    auto parent = object->getParent();
    cocos2d::Vec2 parentSize = parent->getSize();
    cocos2d::Vec2 size = object->getSize();
    cocos2d::Vec2 targetPos = (parentSize / 2) - (size / 2);
    object->setPosition(targetPos.x, targetPos.y);
}
