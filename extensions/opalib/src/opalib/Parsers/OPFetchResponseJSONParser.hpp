//
// Created by Talles Borges  on 28/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPFETCHRESPONSEJSONPARSER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPFETCHRESPONSEJSONPARSER_HPP_

#include "../Json/OPJson.hpp"
#include "../Parsers/OPJsonParser.hpp"
#include "../Misc/OPMacros.hpp"
#include "../Models/Responses/OPFetchResponse.hpp"

struct OPFetchResponseJSONParserParams
{
    std::unique_ptr<OPJsonParser<OPKit>> kitParser;
    std::string kitsKey;

    std::unique_ptr<OPJsonParser<OPCategory>> categoryParser{nullptr};
    std::string categoriesKey;

    std::unique_ptr<OPJsonParser<OPKitCategory>> kitCategoryParser{nullptr};
    std::string kitCategoriesKey;

    std::unique_ptr<OPJsonParser<OPGroupCategory>> groupCategoryParser{nullptr};
    std::string groupCategoriesKey;

    std::string timestampKey;
};

class OPFetchResponseJSONParser : public OPJsonParser<OPFetchResponse>
{
public:
    OPFetchResponseJSONParser(std::unique_ptr<OPFetchResponseJSONParserParams> params);

    virtual OPFetchResponse* createObject() { return OPFetchResponse::create(); }
    
    virtual OPFetchResponse* parse(const rapidjson::Value& value) override;
    virtual rapidjson::Value toJson(OPFetchResponse* t, rapidjson::Document& document) override;

    virtual OPFetchResponseJSONParserParams* getParams() const;
    OPJsonParser<OPKit>* getKitParser() { return _params->kitParser.get(); }

protected:
    std::unique_ptr<OPFetchResponseJSONParserParams> _params;
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPFETCHRESPONSEJSONPARSER_HPP_
