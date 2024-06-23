//
// Created by Talles Borges  on 04/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITLISTCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITLISTCOMPONENT_HPP_

#include "../Models/OPKit.hpp"
#include "OPKitComponent.hpp"

class OPKitListComponent;

typedef std::function<void(OPKitListComponent*, OPKitComponent*, OPKit*, int)> SEL_CustomRenderKitItem;
typedef std::function<void(OPKit*, OPKitListComponent* list)> CustomKitSelectedHandler;

struct OPKitListComponentParams : public virtual OPFairyComponentParams
{
    std::string listId;
    std::string textFieldId;

    std::string locationName;
    //TODO: REFACTOR IT, maybe a dictionary ?
    std::string categoryName;

    bool isVirtual = true;
    uint8_t nonVirtualItems = 5;

    std::function<void(fairygui::GTextField*, fairygui::GComponent*)> onTextFieldChange = nullptr;
    CustomKitSelectedHandler customKitSelectedHandler = nullptr;
    SEL_CustomRenderKitItem customRenderItem = nullptr;
};

class OPKitListComponent : public virtual OPFairyComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPKitListComponent)

    void setItems(cocos2d::Vector<OPKit*> items);
    void appendItems(const cocos2d::Vector<OPKit*>& items);

    void setTitle(const std::string& title);

    void setLocationName(const std::string& name);

    void refreshList();

    CC_SYNTHESIZE(SEL_CustomRenderKitItem, _customRenderItem, CustomRenderKitItem)

    void setCustomSelectedKitHandler(const CustomKitSelectedHandler& customSelectedKitHandler);
    
    fairygui::GList* getItemsList() const;
    const std::string& getText();
    const cocos2d::Vector<OPKit*>& getItems();

protected:
    void onConstruct() override;
    void linkComponents();

    fairygui::GList* _itemsList;
    fairygui::GTextField* _textField;

    CustomKitSelectedHandler _customSelectedKitHandler;

    cocos2d::Vector<OPKit*> _items;

    virtual void renderListItem(int index, fairygui::GObject* obj);
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITLISTCOMPONENT_HPP_
