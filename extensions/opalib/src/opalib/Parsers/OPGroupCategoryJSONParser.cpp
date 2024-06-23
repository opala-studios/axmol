//
// Created by Talles Borges  on 02/06/20.
//

#include "OPGroupCategoryJSONParser.hpp"
#include "Opalib.h"


OPGroupCategoryJSONParser::OPGroupCategoryJSONParser()
{
    _params = std::make_unique<OPGroupCategoryJSONParserParams>();
}

bool OPGroupCategoryJSONParser::init(std::unique_ptr<OPGroupCategoryJSONParserParams> params)
{
    _params = std::move(params);

    return true;
}

OPGroupCategory* OPGroupCategoryJSONParser::parse(const rapidjson::Value& value)
{
    OPGroupCategory* groupCategory = OPGroupCategory::create();
    groupCategory->setId(OPJson::getString(value, "id"));
    groupCategory->setGroupId(OPJson::getString(value, "groupId"));
    groupCategory->setCategoryId(OPJson::getString(value, "categoryId"));
    groupCategory->setOrder(OPJson::getInt(value, "order"));
    groupCategory->setIsDeleted(OPJson::getBool(value, _params->isDeletedKey.c_str()));

    return groupCategory;
}

rapidjson::Value OPGroupCategoryJSONParser::toJson(OPGroupCategory* groupCategory, rapidjson::Document& document)
{
    rapidjson::Value value = OPJson::object();

    OPJson::set(document, value, "id", groupCategory->getId());
    OPJson::set(document, value, "groupId", groupCategory->getGroupId());
    OPJson::set(document, value, "categoryId", groupCategory->getCategoryId());
    OPJson::set(document, value, "order", groupCategory->getOrder());

    return value;
}
