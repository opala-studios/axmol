//
// Created by Talles Borges  on 27/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPKITJSONPARSER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPKITJSONPARSER_HPP_

#include <JuceHeader.h>

#include "../Json/OPJson.hpp"
#include "../Parsers/OPJsonParser.hpp"
#include "../Models/OPKit.hpp"

struct OPKitJSONParserParams
{
    std::string nameKey = "name";
    std::string authorKey = "authorName";
    std::string musicKey = "musicName";
    std::string deletedKey = "isDeleted";
};

class OPKitJSONParser : public OPJsonParser<OPKit>
{
public:
    
    OPKitJSONParser();
    
    void setParams(std::unique_ptr<OPKitJSONParserParams>);

    OPKit* parse(const rapidjson::Value& value) override;

    rapidjson::Value toJson(OPKit* kit, rapidjson::Document& document) override;

    virtual std::string parseKitId(const rapidjson::Value& value);
    virtual void setKitId(OPKit* kit, rapidjson::Document& document, rapidjson::Value& value);

protected:
    virtual OPKit* createObject() = 0;
    std::unique_ptr<OPKitJSONParserParams> _params;
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_PARSERS_OPKITJSONPARSER_HPP_
