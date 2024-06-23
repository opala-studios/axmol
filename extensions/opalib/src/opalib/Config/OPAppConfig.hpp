//
//  OPAppConfig.hpp
//  Opalib
//
//  Created by Adilson Tavares on 16/07/19.
//  Copyright © 2019 Opala Studios. All rights reserved.
//

#ifndef OPAppConfig_hpp
#define OPAppConfig_hpp

#include <cocos2d.h>
#include <json/document-wrapper.h>

class OPAppConfig : public cocos2d::Ref
{    
public:

    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, title, Title);
    CC_SYNTHESIZE_READONLY(float, contentScaleFactor, ContentScaleFactor);
    CC_SYNTHESIZE_READONLY(bool, usesFairyGUI, UsesFairyGUI);
    CC_SYNTHESIZE_READONLY(bool, usesCocos, UsesCocos);
    CC_SYNTHESIZE_READONLY(cocos2d::Size, designResolution, DesignResolution);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, imagesPath, ImagesPath);
    CC_SYNTHESIZE_READONLY(ResolutionPolicy, resolutionPolicy, ResolutionPolicy);
	
	struct
	{
		std::string projectName;
		std::string action;
		int scenario = 0;
	} testConfig;
    
    bool initWithJson(const rapidjson::Value& json);
    static OPAppConfig* create(const rapidjson::Value& json);
};

#endif /* OPAppConfig_hpp */
