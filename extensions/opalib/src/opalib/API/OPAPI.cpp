//
// Created by Talles Borges  on 24/07/20.
//

#include "OPAPI.hpp"

#include "../Debug/OPDebug.hpp"
#include "../Managers/OPAppManager.hpp"

OPAPI::OPAPI(int version) : _version(version) {}

std::string OPAPI::makeUrl(const std::string& path)
{
    auto pathResolver = OPAppManager::shared()->getPathResolver();
    return pathResolver->getApiUrl() + path;
}

cocos2d::network::HttpRequest* OPAPI::fetchKits(int64_t timestamp, const SEL_FetchResponseHandler& handler)
{
    auto pathResolver = OPAppManager::shared()->getPathResolver();
    auto fetchPath = pathResolver->getFetchUrlPath(timestamp, _version);

    auto request = makeGETRequest(fetchPath);
    sendJSONRequest(
        request,
        [this, handler](const rapidjson::Document& document) {
            auto fetchParser = OPAppManager::shared()->getFetchParser();
            auto response = fetchParser->parse(document);
            handler(response);
        },
        [handler](int errorCode, const std::string& errorStr) {
            OPDebug::error("OPPadsApi", errorStr);
            if (handler)
                handler(nullptr);
        });

    return request;
}
