#ifndef OPAPI_hpp
#define OPAPI_hpp

#include <cocos2d.h>
#include <json/document-wrapper.h>
#include <network/HttpClient.h>
#include <network/HttpRequest.h>
#include <functional>

class OPBaseAPI
{
protected:

    virtual std::string makeUrl(const std::string& path) = 0;

    typedef std::function<void(const rapidjson::Document& document)> SEL_JsonResponseHandler;
    typedef std::function<void(const std::string& str)> SEL_StringResponseHandler;
    typedef std::function<void(const std::vector<char>&)> SEL_DataResponseHandler;
    typedef std::function<void(int errorCode, const std::string& errorStr)> SEL_ErrorHandler;

    cocos2d::network::HttpRequest* makeGETRequest(const std::string& path);
	cocos2d::network::HttpRequest* makePOSTRequest(const std::string& path, const std::string& data, const std::string& token);
    
    void sendJSONRequest(cocos2d::network::HttpRequest* request, const SEL_JsonResponseHandler& handler, const SEL_ErrorHandler& error = 0);
    void sendStringRequest(cocos2d::network::HttpRequest* request, const SEL_StringResponseHandler& handler, const SEL_ErrorHandler& error = 0);
    void sendDataRequest(cocos2d::network::HttpRequest* request, const SEL_DataResponseHandler& handler, const SEL_ErrorHandler& error = 0);
};

#endif
