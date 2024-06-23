//
// Created by Talles Borges  on 27/07/20.
//

#include "OPKitJSONParser.hpp"

OPKitJSONParser::OPKitJSONParser()
{
    auto params = std::make_unique<OPKitJSONParserParams>();
    _params = std::move(params);
}

std::string OPKitJSONParser::parseKitId(const rapidjson::Value& value)
{
    return OPJson::getString(value, "id");
}

void OPKitJSONParser::setKitId(OPKit* kit, rapidjson::Document& document, rapidjson::Value& value)
{
    OPJson::set(document, value, "id", kit->getId());
}

OPKit* OPKitJSONParser::parse(const rapidjson::Value& value)
{
    auto kit = createObject();

    kit->setId(parseKitId(value));
    kit->setName(OPJson::getString(value, _params->nameKey.c_str()));
    kit->setAuthorName(OPJson::getString(value, _params->authorKey.c_str()));
    kit->setMusicName(OPJson::getString(value, _params->musicKey.c_str()));
    kit->setIsDeleted(OPJson::getBool(value, _params->deletedKey.c_str()));

    return kit;
}

rapidjson::Value OPKitJSONParser::toJson(OPKit* kit, rapidjson::Document& document)
{
    rapidjson::Value value = OPJson::object();

    setKitId(kit, document, value);
    OPJson::set(document, value, _params->nameKey.c_str(), kit->getName());
    OPJson::set(document, value, _params->authorKey.c_str(), kit->getAuthorName());
    OPJson::set(document, value, _params->musicKey.c_str(), kit->getMusicName());

    return value;
}

void OPKitJSONParser::setParams(std::unique_ptr<OPKitJSONParserParams> params)
{
    _params = std::move(params);
}
