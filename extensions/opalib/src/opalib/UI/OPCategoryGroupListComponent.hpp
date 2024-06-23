//
// Created by Talles Borges  on 05/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYGROUPLISTCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYGROUPLISTCOMPONENT_HPP_

#include "OPCategoryListComponent.hpp"

struct OPCategoryGroupListComponentParams : public OPCategoryListComponentParams
{
    std::string titleId;
    std::string indicatorId;
    std::string indicatorControllerId;
    std::string indicatorDownId;
    std::string indicatorUpId;
};

class OPCategoryGroupListComponent : public OPCategoryListComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPCategoryGroupListComponent)

    void setCategory(OPCategory* category) override;

    ~OPCategoryGroupListComponent() override;

protected:
    void onConstruct() override;
    void doSpecialEffect(fairygui::EventContext* context) override;

    fairygui::GTextField* _titleTextField;
    fairygui::GList* _indicatorList;

    int _currentIndex = 1;

    long _updateIndicatorInvoke = -1;
};

class OPFeaturedCategoryGroupListComponent : public OPCategoryGroupListComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPFeaturedCategoryGroupListComponent)
private:
    void renderListItem(int index, fairygui::GObject* obj) override;

protected:
    float _itemWidthProportion = 0.6f;
    int _columnGap = 80;
    float _featuredItemHeight = 0;
    float _featuredItemWidth = 0;

    void onConstruct() override;

public:
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYGROUPLISTCOMPONENT_HPP_
