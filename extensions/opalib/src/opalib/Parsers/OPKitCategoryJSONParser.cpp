//
// Created by Talles Borges  on 02/06/20.
//

#include "OPKitCategoryJSONParser.hpp"
#include "Opalib.h"

OPKitCategoryJSONParser::OPKitCategoryJSONParser()
{
    _params = std::make_unique<OPKitCategoryJSONParserParams>();
}

bool OPKitCategoryJSONParser::init(std::unique_ptr<OPKitCategoryJSONParserParams> params)
{
    _params = std::move(params);

    return true;
}

OPKitCategory* OPKitCategoryJSONParser::parse(const rapidjson::Value& value)
{
    OPKitCategory* kitCategory = OPKitCategory::create();
    kitCategory->setCategoryId(OPJson::getString(value, "categoryId"));
    kitCategory->setKitId(parseKitId(value));
    kitCategory->setOrder(OPJson::getInt(value, "order"));
    kitCategory->setIsDeleted(OPJson::getBool(value, _params->isDeletedKey.c_str()));
    kitCategory->setId(OPJson::getString(value, "id"));

    return kitCategory;
}

rapidjson::Value OPKitCategoryJSONParser::toJson(OPKitCategory* kitCategory, rapidjson::Document& document)
{
    rapidjson::Value value = OPJson::object();

    OPJson::set(document, value, "id", kitCategory->getId());
    OPJson::set(document, value, "categoryId", kitCategory->getCategoryId());
    setKitId(kitCategory, document, value);
    OPJson::set(document, value, "order", kitCategory->getOrder());

    return value;
}

void OPKitCategoryJSONParser::setKitId(OPKitCategory* kitCategory, rapidjson::Document& document, rapidjson::Value& value)
{
    OPJson::set(document, value, "kitId", kitCategory->getKitId());
}

std::string OPKitCategoryJSONParser::parseKitId(const rapidjson::Value& value)
{
    return OPJson::getString(value, "kitId");
}
