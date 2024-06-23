//
//  OPAppConfig.cpp
//  Opalib
//
//  Created by Adilson Tavares on 16/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#include "OPAppConfig.hpp"
#include "../Json/OPJson.hpp"

USING_NS_CC;

bool OPAppConfig::initWithJson(const rapidjson::Value& json)
{
    title = OPJson::getString(json, "title");
    designResolution = OPJson::getSize(json, "resolution");
    contentScaleFactor = OPJson::getFloat(json, "scale_factor");
    usesFairyGUI = OPJson::getBool(json, "fairygui");
    usesCocos = OPJson::getBool(json, "cocos") || !usesFairyGUI;
    resolutionPolicy = ResolutionPolicy::NO_BORDER;
	
	if(OPJson::hasObject(json, "test_config"))
	{
		auto tc = json["test_config"].GetObject();
		testConfig.projectName = tc["project_name"].GetString();
		testConfig.action = tc["action"].GetString();
		testConfig.scenario = tc["scenario"].GetInt();
	}
    
    auto resolutionPolicyString = OPJson::getString(json, "resolution_policy");
    if (resolutionPolicyString.empty())
    {
        if (usesFairyGUI) {
            resolutionPolicy = ResolutionPolicy::FIXED_HEIGHT;
        }
        else {
            resolutionPolicy = ResolutionPolicy::NO_BORDER;
        }
    }
    
    if (resolutionPolicyString == "exact_fit") {
        resolutionPolicy = ResolutionPolicy::EXACT_FIT;
    }
    else if (resolutionPolicyString == "no_border") {
        resolutionPolicy = ResolutionPolicy::NO_BORDER;
    }
    else if (resolutionPolicyString == "show_all") {
        resolutionPolicy = ResolutionPolicy::SHOW_ALL;
    }
    else if (resolutionPolicyString == "fixed_height") {
        resolutionPolicy = ResolutionPolicy::FIXED_HEIGHT;
    }
    else if (resolutionPolicyString == "fixed_width") {
        resolutionPolicy = ResolutionPolicy::FIXED_WIDTH;
    }
    
    if (title.empty()) {
        title = "Opala Application";
    }
    
    if (designResolution.width == 0 && designResolution.height == 0) {
        designResolution = Size(Director::getInstance()->getWinSizeInPixels().width, Director::getInstance()->getWinSizeInPixels().height);
    }
    
    imagesPath = OPJson::getString(json, "imagesPath");
    if (imagesPath.empty()) {
        imagesPath = "ui";
    }
    
    return true;
}

OPAppConfig* OPAppConfig::create(const rapidjson::Value& json)
{
    auto config = new(std::nothrow) OPAppConfig();
    if (config && config->initWithJson(json))
    {
        config->autorelease();
        return config;
    }
    
    delete config;
    config = 0;
    return 0;
}
