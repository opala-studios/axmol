//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_MODELS_OPGROUPCATEGORY_HPP_
#define OPALIBTEMPLATE_CLASSES_MODELS_OPGROUPCATEGORY_HPP_

#include <cocos2d.h>
#include "OPCategory.hpp"

class OPGroupCategory : public cocos2d::Ref
{
public:
    CREATE_FUNC(OPGroupCategory);
    CC_SYNTHESIZE(OPCategory*, _group, Group);
    CC_SYNTHESIZE(OPCategory*, _category, Category);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _id, Id);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _groupId, GroupId);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _categoryId, CategoryId);
    CC_SYNTHESIZE(int, _order, Order);
    CC_SYNTHESIZE(bool, _isDeleted, IsDeleted);
    bool init() { return true; };
};

#endif // OPALIBTEMPLATE_CLASSES_MODELS_OPGROUPCATEGORY_HPP_
