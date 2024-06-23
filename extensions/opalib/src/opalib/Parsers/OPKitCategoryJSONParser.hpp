//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPKITCATEGORYJSONPARSER_HPP_
#define OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPKITCATEGORYJSONPARSER_HPP_

#include "../Models/OPKitCategory.hpp"
#include "../Json/OPJson.hpp"
#include "../Parsers/OPJsonParser.hpp"

struct OPKitCategoryJSONParserParams
{
    std::string isDeletedKey = "isDeleted";
};

class OPKitCategoryJSONParser : public OPJsonParser<OPKitCategory>
{
public:
    OPKitCategoryJSONParser();
    bool init(std::unique_ptr<OPKitCategoryJSONParserParams> params);

    OPKitCategory* parse(const rapidjson::Value& value) override;
    rapidjson::Value toJson(OPKitCategory* t, rapidjson::Document& document) override;

    virtual std::string parseKitId(const rapidjson::Value& value);
    virtual void setKitId(OPKitCategory* kitCategory, rapidjson::Document& document, rapidjson::Value& value);

protected:
    std::unique_ptr<OPKitCategoryJSONParserParams> _params;
};

#endif // OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPKITCATEGORYJSONPARSER_HPP_
