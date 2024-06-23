//
// Created by Talles Borges  on 26/06/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_JSON_OPJSONPARSER_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_JSON_OPJSONPARSER_HPP_

#include "../Json/OPJson.hpp"
#include "cocos2d.h"

template <class T>
class OPJsonParser // template definition
{
public:
    virtual T* parse(const rapidjson::Value& value) = 0;
    virtual rapidjson::Value toJson(T*, rapidjson::Document&) = 0;

    virtual ~OPJsonParser() {}
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_JSON_OPJSONPARSER_HPP_
