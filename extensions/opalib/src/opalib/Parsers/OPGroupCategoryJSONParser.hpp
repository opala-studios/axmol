//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPGROUPCATEGORYJSONPARSER_HPP_
#define OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPGROUPCATEGORYJSONPARSER_HPP_

#include "../Models/OPGroupCategory.hpp"
#include "../Json/OPJson.hpp"
#include "../Parsers/OPJsonParser.hpp"

struct OPGroupCategoryJSONParserParams
{
    std::string isDeletedKey = "isDeleted";
};

class OPGroupCategoryJSONParser : public OPJsonParser<OPGroupCategory>
{
public:
    OPGroupCategoryJSONParser();
    bool init(std::unique_ptr<OPGroupCategoryJSONParserParams>);

    OPGroupCategory* parse(const rapidjson::Value& value) override;
    rapidjson::Value toJson(OPGroupCategory* t, rapidjson::Document& document) override;

protected:
    std::unique_ptr<OPGroupCategoryJSONParserParams> _params;
};

#endif // OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPGROUPCATEGORYJSONPARSER_HPP_
