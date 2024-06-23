//
//  OPTestLabAPI.hpp
//  opalib-ios
//
//  Created by Allison Lindner on 12/08/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPTestLabAPI_hpp
#define OPTestLabAPI_hpp

#include <cocos2d.h>

#include "../API/OPBaseAPI.hpp"
#include "../Parsers/OPJsonParser.hpp"
#include "../Misc/OPMacros.hpp"
#include "../Models/Responses/OPFetchResponse.hpp"
#include "../Parsers/OPFetchResponseJSONParser.hpp"
#include "../TestManager/OPTest.hpp"

typedef std::function<void(opalib::OPTest testResponse)> SEL_LoadAppTestHandler;
typedef std::function<void(bool success)> SEL_SaveAppTestHandler;

class OPTestLabAPI : public OPBaseAPI
{
public:
	explicit OPTestLabAPI(int version);
	cocos2d::network::HttpRequest* saveAppTest(rapidjson::Document* document, std::string projectName, int testNumber, const SEL_SaveAppTestHandler& handler);
	cocos2d::network::HttpRequest* loadAppTest(std::string projectName, int testNumber, const SEL_LoadAppTestHandler& handler);
	
protected:
	std::string makeUrl(const std::string& path) override;

private:
	int _version;
	const std::string _token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJkZWZhdWx0In0.FyI8lgGm-YBLIhRo3Weaz9VQztsODPmtKmrz_mlUZsg";
};

#endif /* OPTestLabAPI_hpp */
