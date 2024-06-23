//
// Created by Talles Borges  on 28/07/20.
//

#ifndef OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_RESPONSES_OPFETCHRESPONSE_HPP_
#define OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_RESPONSES_OPFETCHRESPONSE_HPP_

#include <cocos2d.h>

#include "../OPCategory.hpp"
#include "../OPGroupCategory.hpp"
#include "../OPKit.hpp"
#include "../OPKitCategory.hpp"

struct OPFetchResponse : public cocos2d::Ref
{
public:
    CREATE_FUNC(OPFetchResponse);
    virtual bool init() { return true; }

    cocos2d::Map<std::string, OPKit*> kits;
    cocos2d::Map<std::string, OPCategory*> categories;
    cocos2d::Map<std::string, OPKitCategory*> kitCategories;
    cocos2d::Map<std::string, OPGroupCategory*> groupCategories;
    int64_t timestamp;
};

#endif // OPALIBTEMPLATE_OPALIB_OPALIB_MODELS_RESPONSES_OPFETCHRESPONSE_HPP_
