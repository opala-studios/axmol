//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPCATEGORYJSONPARSER_HPP_
#define OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPCATEGORYJSONPARSER_HPP_

#include "../Models/OPCategory.hpp"
#include "../Json/OPJson.hpp"
#include "../Parsers/OPJsonParser.hpp"
#include "../Misc/OPMacros.hpp"

struct OPCategoryJsonParserParams
{
    std::string backgroundUrlKey = "backgroundUrl";
    std::string thumbUrlKey = "urlThumb";
    std::string referencedKitKey = "referencedKitId";
    std::string referenceUrlKey = "referencedUrl";
    std::string referenceCategoryKey = "referencedCategoryId";
    std::string isDeletedKey = "isDeleted";
};

class OPCategoryJSONParser : public OPJsonParser<OPCategory>
{
public:
    OPCategoryJSONParser();
    bool init(std::unique_ptr<OPCategoryJsonParserParams> params);

    OPCategory* parse(const rapidjson::Value& value) override;
    rapidjson::Value toJson(OPCategory* t, rapidjson::Document& document) override;

protected:
    virtual std::string parseReferencedKitId(const rapidjson::Value& value);
    virtual void setReferencedKitId(OPCategory* t, rapidjson::Document& document, rapidjson::Value& value);

    std::unique_ptr<OPCategoryJsonParserParams> _params;
};

#endif // OPALIBTEMPLATE_CLASSES_NETWORK_PARSERS_OPCATEGORYJSONPARSER_HPP_
