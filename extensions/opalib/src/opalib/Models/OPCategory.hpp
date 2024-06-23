//
// Created by Talles Borges  on 02/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_MODELS_OPCATEGORY_HPP_
#define OPALIBTEMPLATE_CLASSES_MODELS_OPCATEGORY_HPP_

#include <string>
#include <cocos2d.h>

class OPCategory : public cocos2d::Ref
{
public:
    bool init() { return true; }
    CREATE_FUNC(OPCategory);

    enum Type
    {
        VERTICAL_KITS = 1,
        HORIZONTAL_KITS = 2,
        GROUP_CATEGORY = 3,
        FEATURED = 100,
        FEATURED_ITEM = 101,
        FEATURED_CATEGORY = 102,
        FEATURED_URL = 103
    };

    CC_SYNTHESIZE_PASS_BY_REF(std::string, _name, Name);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _urlThumb, URLThumb);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _urlBackground, URLBackground);
    CC_SYNTHESIZE_PASS_BY_REF(Type, _type, Type);
    CC_SYNTHESIZE_PASS_BY_REF(int, _order, Order);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _id, Id);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _referencedId, ReferencedId)
    CC_SYNTHESIZE_PASS_BY_REF(bool, _showInFeed, ShowInFeed);
    CC_SYNTHESIZE_PASS_BY_REF(bool, _isAcendingOrder, IsAcendingOrder);
    CC_SYNTHESIZE_PASS_BY_REF(bool, _isDeleted, IsDeleted);
};

namespace opalib {
namespace Event {

struct CategorySelected
{
    OPCategory* category;
};
} // namespace Event
} // namespace opalib

#endif // OPALIBTEMPLATE_CLASSES_MODELS_OPCATEGORY_HPP_
