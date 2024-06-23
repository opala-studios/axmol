//
//  OPTestLabAPI.cpp
//  opalib-ios
//
//  Created by Allison Lindner on 12/08/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPTestLabAPI.hpp"

#include "../Debug/OPDebug.hpp"

#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <iostream>

OPTestLabAPI::OPTestLabAPI(int version) : _version(version) {}

std::string OPTestLabAPI::makeUrl(const std::string& path)
{
	return "https://api-testlab.opalastudios.com" + path;
}

cocos2d::network::HttpRequest* OPTestLabAPI::saveAppTest(rapidjson::Document* document, std::string projectName, int testNumber, const SEL_SaveAppTestHandler &handler)
{
	auto fetchPath = "/api/" + projectName + "/" + std::to_string(testNumber);
	
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	
	document->Accept(writer);
	
	auto request = makePOSTRequest(fetchPath, std::string(buffer.GetString()), _token);
	sendJSONRequest(
		request,
		[this, handler](const rapidjson::Document& document) {
			handler(true);
		},
		[](int errorCode, const std::string& errorStr) {
			OPDebug::error("OPPadsApi", errorStr);
		});
	
	return request;
	
	return nullptr;
}

cocos2d::network::HttpRequest* OPTestLabAPI::loadAppTest(std::string projectName, int testNumber, const SEL_LoadAppTestHandler &handler)
{
	auto fetchPath = "/api/" + projectName + "/" + std::to_string(testNumber);

	auto request = makeGETRequest(fetchPath);
	sendJSONRequest(
		request,
		[this, handler](const rapidjson::Document& document) {
			opalib::OPTest test;
			test.parse(document);
		
			handler(test);
		},
		[](int errorCode, const std::string& errorStr) {
			OPDebug::error("OPPadsApi", errorStr);
		});
	
	return request;
}
