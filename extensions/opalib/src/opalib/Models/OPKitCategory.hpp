//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_MODELS_OPKITCATEGORY_HPP_
#define OPALIBTEMPLATE_CLASSES_MODELS_OPKITCATEGORY_HPP_

#include <cocos2d.h>
#include "OPKit.hpp"
#include "OPCategory.hpp"

class OPKitCategory : public cocos2d::Ref
{
public:
    CREATE_FUNC(OPKitCategory)
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _id, Id);
    
    CC_SYNTHESIZE(OPKit*, _kit, Kit);
    CC_SYNTHESIZE(OPCategory*, _category, Category);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _kitId, KitId)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _categoryId, CategoryId)
    
    CC_SYNTHESIZE(int, _order, Order);
    CC_SYNTHESIZE(bool, _isDeleted, IsDeleted);
    
    bool init() { return true; };
};

#endif // OPALIBTEMPLATE_CLASSES_MODELS_OPKITCATEGORY_HPP_
