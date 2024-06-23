//
// Created by Talles Borges  on 04/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYLISTCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYLISTCOMPONENT_HPP_

#include "OPCategoryComponent.hpp"

struct OPCategoryListComponentParams : public OPCategoryComponentParams
{
    std::string listId;
    bool shouldZoomItems;
};

class OPCategoryListComponent : public OPCategoryComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPCategoryListComponent)

    void setItems(const cocos2d::Vector<OPCategory*>& items);

protected:
    fairygui::GList* _itemsList;

    cocos2d::Vector<OPCategory*> _items;

    virtual void renderListItem(int index, fairygui::GObject* obj);

    virtual void doSpecialEffect(fairygui::EventContext* context);

    void onConstruct() override;
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYLISTCOMPONENT_HPP_
