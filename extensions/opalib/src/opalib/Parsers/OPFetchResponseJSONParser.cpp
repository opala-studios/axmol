//
// Created by Talles Borges  on 28/07/20.
//

#include "OPFetchResponseJSONParser.hpp"
#include "OPCategoryJSONParser.hpp"
#include "OPGroupCategoryJSONParser.hpp"
#include "OPKitCategoryJSONParser.hpp"
#include "../Debug/OPDebug.hpp"
#include "Opalib.h"

OPFetchResponseJSONParser::OPFetchResponseJSONParser(std::unique_ptr<OPFetchResponseJSONParserParams> params) : _params(std::move(params))
{
    if (_params->categoryParser == nullptr) {
        _params->categoryParser = std::make_unique<OPCategoryJSONParser>();
    }

    if (_params->kitCategoryParser == nullptr) {
        _params->kitCategoryParser = std::make_unique<OPKitCategoryJSONParser>();
    }

    if (_params->groupCategoryParser == nullptr) {
        _params->groupCategoryParser = std::make_unique<OPGroupCategoryJSONParser>();
    }
}

OPFetchResponse* OPFetchResponseJSONParser::parse(const rapidjson::Value& value)
{
    OPDebug::log("FetchJSONParser", "starting parse fetch document");
    auto params = _params.get();

    auto response = createObject();

    if (OPJson::hasArray(value, params->kitsKey)) {
        for (const auto& kitJson : value[params->kitsKey.c_str()].GetArray()) {
            auto kit = _params->kitParser->parse(kitJson);
            if (kit) {
                response->kits.insert(kit->getId(), kit);
            }
        }
    }

    // setup categories
    if (OPJson::hasArray(value, params->categoriesKey)) {
        for (const auto& categoryJson : value[params->categoriesKey.c_str()].GetArray()) {
            auto category = _params->categoryParser->parse(categoryJson);
            if (category) {
                response->categories.insert(category->getId(), category);
            }
        }
    }

    // setup categoryKits
    if (OPJson::hasArray(value, params->kitCategoriesKey)) {
        for (const auto& categoryKitJson : value[params->kitCategoriesKey.c_str()].GetArray()) {
            auto kitCategory = _params->kitCategoryParser->parse(categoryKitJson);
            if (kitCategory) {
                response->kitCategories.insert(kitCategory->getId(), kitCategory);
            }
        }
    }

    // setup groupCategories
    if (OPJson::hasArray(value, params->groupCategoriesKey)) {
        for (const auto& json : value[params->groupCategoriesKey.c_str()].GetArray()) {
            auto groupCategory = _params->groupCategoryParser->parse(json);
            if (groupCategory) {
                response->groupCategories.insert(groupCategory->getId(), groupCategory);
            }
        }
    }

    response->timestamp = OPJson::getLong(value, _params->timestampKey.c_str());

    return response;
}

rapidjson::Value OPFetchResponseJSONParser::toJson(OPFetchResponse* fetchResponse, rapidjson::Document& document)
{
    rapidjson::Value kits = OPJson::array();

    for (const auto& pair : fetchResponse->kits) {
        if (pair.second->isDeleted()) {
            continue;
        }
        auto item = _params->kitParser->toJson(pair.second, document);
        OPJson::append(document, kits, item);
    }

    // categories
    rapidjson::Value categories = OPJson::array();
    for (const auto& pair : fetchResponse->categories) {
        if (pair.second->getIsDeleted()) {
            continue;
        }
        auto item = _params->categoryParser->toJson(pair.second, document);
        OPJson::append(document, categories, item);
    }

    // kitCategories
    rapidjson::Value categoryKits = OPJson::array();
    for (const auto& pair : fetchResponse->kitCategories) {
        if (pair.second->getIsDeleted()) {
            continue;
        }
        auto item = _params->kitCategoryParser->toJson(pair.second, document);
        OPJson::append(document, categoryKits, item);
    }

    // groupCategories
    rapidjson::Value categoryGroups = OPJson::array();
    for (const auto& pair : fetchResponse->groupCategories) {
        if (pair.second->getIsDeleted()) {
            continue;
        }
        auto item = _params->groupCategoryParser->toJson(pair.second, document);
        OPJson::append(document, categoryGroups, item);
    }

    OPJson::set(document, _params->kitsKey, kits);
    OPJson::set(document, _params->categoriesKey, categories);
    OPJson::set(document, _params->kitCategoriesKey, categoryKits);
    OPJson::set(document, _params->groupCategoriesKey, categoryGroups);
    OPJson::set(document, _params->timestampKey, fetchResponse->timestamp);

    return OPJson::object();
}

OPFetchResponseJSONParserParams* OPFetchResponseJSONParser::getParams() const
{
    return _params.get();
}
