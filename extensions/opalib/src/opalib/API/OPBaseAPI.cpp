#include "OPBaseAPI.hpp"

#include "../Debug/OPDebug.hpp"
#include "../Json/OPJson.hpp"
#include "../Misc/OPCocosThread.hpp"

USING_NS_CC;

network::HttpRequest* OPBaseAPI::makeGETRequest(const std::string& path)
{
    //TODO: Investigate memory leak here, need to manage retain/release the request.
    auto request = new network::HttpRequest();
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setUrl(makeUrl(path));

    return request;
}

network::HttpRequest* OPBaseAPI::makePOSTRequest(const std::string& path, const std::string& data, const std::string& token)
{
	//TODO: Investigate memory leak here, need to manage retain/release the request.
	auto request = new network::HttpRequest();
	request->setRequestType(network::HttpRequest::Type::POST);
	request->setUrl(makeUrl(path));
	
	std::vector<std::string> headers;
	headers.push_back("accept: */*");
	headers.push_back("Authorization: Bearer " + token);
	headers.push_back("Content-Type: application/json");
	
	request->setHeaders(headers);
	
	request->setRequestData(data.data(), data.size());

	return request;
}

void OPBaseAPI::sendJSONRequest(cocos2d::network::HttpRequest* request, const SEL_JsonResponseHandler& handler, const SEL_ErrorHandler& error)
{
    sendStringRequest(request, [handler, error](const std::string& str)
    {
        rapidjson::Document document;
        
        if (OPJson::parse(str, document)) {
            handler(document);
        }
        else if (error) {
            error(-1, "Could not parse JSON file.");
        }
    },
    error);
}

void OPBaseAPI::sendStringRequest(cocos2d::network::HttpRequest* request, const SEL_StringResponseHandler& handler, const SEL_ErrorHandler& error)
{
    sendDataRequest(request, [handler](const std::vector<char>& data)
    {
        auto str = std::string(data.begin(), data.end());
        handler(str);
    }, 
    error);
}

void OPBaseAPI::sendDataRequest(cocos2d::network::HttpRequest* request, const SEL_DataResponseHandler& handler, const SEL_ErrorHandler& error)
{
    OPDebug::log("OPAPI", "Starting request to url " + std::string(request->getUrl()));

    request->setResponseCallback([handler, error](network::HttpClient* client, network::HttpResponse* response)
    {
        OPDebug::log("OPAPI", "Request finished with success = " + std::to_string(response->isSucceed()) + ". Response code = " + std::to_string(response->getResponseCode()));
        OP_LOG_VERB("Response with tag " + std::string(response->getHttpRequest()->getTag()) + "finished");

        if (response->isSucceed()) 
        {
            auto data = *response->getResponseData();

            if (handler) {
                handler(data);
            }
        }
        else
        {
            auto errorCode = response->getResponseCode();
            auto errorMessage = std::string(response->getErrorBuffer());

            if (error) {
                OPDebug::error("OPAPI", "Code: " + std::to_string(errorCode) + errorMessage);
                error((int)errorCode, errorMessage);
            }
        }
    });

    network::HttpClient::getInstance()->send(request);
}
