//
// Created by Talles Borges  on 02/06/20.
//

#include "OPCategoryJSONParser.hpp"
#include "Opalib.h"

OPCategoryJSONParser::OPCategoryJSONParser()
{
    _params = std::make_unique<OPCategoryJsonParserParams>();
}

bool OPCategoryJSONParser::init(std::unique_ptr<OPCategoryJsonParserParams> params)
{
    _params = std::move(params);

    return true;
}

OPCategory* OPCategoryJSONParser::parse(const rapidjson::Value& value)
{
    OPCategory* category = OPCategory::create();

    category->setId(OPJson::getString(value, "id"));

    std::string jsonCategoryKey = OPJson::getString(value, "name");
    category->setName(jsonCategoryKey);

    int intType = OPJson::getInt(value, "type");
    OPCategory::Type type;
    switch (intType) {
        case 100:
            type = OPCategory::Type::FEATURED;
            break;
        case 1:
            type = OPCategory::Type::VERTICAL_KITS;
            break;
        case 2:
            type = OPCategory::Type::HORIZONTAL_KITS;
            break;
        case 3:
            type = OPCategory::Type::GROUP_CATEGORY;
            break;
        case 101:
            type = OPCategory::Type::FEATURED_ITEM;
            category->setReferencedId(parseReferencedKitId(value));
            break;
        case 102:
            type = OPCategory::Type::FEATURED_CATEGORY;
            category->setReferencedId(OPJson::getString(value, _params->referenceCategoryKey.c_str()));
            break;
        case 103:
            type = OPCategory::Type::FEATURED_URL;
            category->setReferencedId(OPJson::getString(value, _params->referenceUrlKey.c_str()));
            break;
        default:
            type = OPCategory::Type::VERTICAL_KITS;
    }

    category->setType(type);
    category->setShowInFeed(OPJson::getBool(value, "showInFeed"));
    category->setOrder(OPJson::getInt(value, "order"));
    category->setURLThumb(OPJson::getString(value, _params->thumbUrlKey.c_str()));
    category->setURLBackground(OPJson::getString(value, _params->backgroundUrlKey.c_str()));
    category->setIsAcendingOrder(OPJson::getBool(value, "sortAscending"));
    category->setIsDeleted(OPJson::getBool(value, _params->isDeletedKey.c_str()));

    return category;
}

rapidjson::Value OPCategoryJSONParser::toJson(OPCategory* category, rapidjson::Document& document)
{
    rapidjson::Value value = OPJson::object();

    OPJson::set(document, value, "id", category->getId());
    OPJson::set(document, value, "name", category->getName());
    OPJson::set(document, value, "showInFeed", category->getShowInFeed());
    OPJson::set(document, value, "order", category->getOrder());
    OPJson::set(document, value, _params->thumbUrlKey, category->getURLThumb());
    OPJson::set(document, value, _params->backgroundUrlKey, category->getURLBackground());
    OPJson::set(document, value, "sortAscending", category->getIsAcendingOrder());
    OPJson::set(document, value, "type", category->getType());

    switch (category->getType()) {
        case OPCategory::Type::FEATURED_ITEM:
            setReferencedKitId(category, document, value);
            break;
        case OPCategory::Type::FEATURED_CATEGORY:
            OPJson::set(document, value, _params->referenceCategoryKey.c_str(), category->getReferencedId());
            break;
        case OPCategory::Type::FEATURED_URL:
            OPJson::set(document, value, _params->referenceUrlKey.c_str(), category->getReferencedId());
            break;
        default:
            break;
    }
    return value;
}

std::string OPCategoryJSONParser::parseReferencedKitId(const rapidjson::Value& value)
{
    return OPJson::getString(value, _params->referencedKitKey.c_str());
}

void OPCategoryJSONParser::setReferencedKitId(OPCategory* category, rapidjson::Document& document, rapidjson::Value& value)
{
    OPJson::set(document, value, _params->referencedKitKey, category->getReferencedId());
}
