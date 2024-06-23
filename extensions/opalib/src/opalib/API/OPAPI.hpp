//
// Created by Talles Borges  on 24/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_API_OPAPI_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_API_OPAPI_HPP_

#include <cocos2d.h>

#include "../API/OPBaseAPI.hpp"
#include "../Parsers/OPJsonParser.hpp"
#include "../Misc/OPMacros.hpp"
#include "../Models/Responses/OPFetchResponse.hpp"
#include "../Parsers/OPFetchResponseJSONParser.hpp"
#include "../TestManager/OPTest.hpp"

typedef std::function<void(OPFetchResponse* fetchResponse)> SEL_FetchResponseHandler;
typedef std::function<void(bool isInEea)> SEL_GDPRResponseHandler;
typedef std::function<void(opalib::OPTest* testResponse)> SEL_LoadAppTestHandler;

class OPAPI : public OPBaseAPI
{
public:
    explicit OPAPI(int version);
    cocos2d::network::HttpRequest* fetchKits(int64_t timestamp, const SEL_FetchResponseHandler& handler);
	cocos2d::network::HttpRequest* saveAppTest(std::string projectName, int testNumber);
	cocos2d::network::HttpRequest* loadAppTest(std::string projectName, int testNumber, const SEL_LoadAppTestHandler& handler);

    // GDPR
    virtual cocos2d::network::HttpRequest* checkGDPRRequirement(const SEL_GDPRResponseHandler& handler) = 0;

protected:
    std::string makeUrl(const std::string& path) override;

private:
    int _version;
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_API_OPAPI_HPP_
