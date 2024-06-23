//
// Created by Talles Borges  on 05/06/20.
//

#include "OPCategoryGroupListComponent.hpp"

#include "../Managers/OPAppManager.hpp"
#include "../Application/OPApplication.hpp"
#include "../Opalib.h"

void OPCategoryGroupListComponent::onConstruct()
{
    OPCategoryListComponent::onConstruct();

    auto params = getParams<OPCategoryGroupListComponentParams>();

    if (!params->titleId.empty()) {
        _titleTextField = getChild(params->titleId)->as<fairygui::GTextField>();
    }

    if (!params->indicatorId.empty()) {
        _indicatorList = getChild(params->indicatorId)->as<fairygui::GList>();
        _indicatorList->setNumItems(0);
        _indicatorList->itemRenderer = [this](int index, fairygui::GObject* item) {
            auto params = getParams<OPCategoryGroupListComponentParams>();
            if (index == _currentIndex) {
                item->as<fairygui::GComponent>()->getController(params->indicatorControllerId)->setSelectedPage(params->indicatorDownId);
            } else {
                item->as<fairygui::GComponent>()->getController(params->indicatorControllerId)->setSelectedPage(params->indicatorUpId);
            }
        };
    }
}

void OPCategoryGroupListComponent::setCategory(OPCategory* category)
{
    OPCategoryListComponent::setCategory(category);

    if (_titleTextField) {
        _titleTextField->setText(OPLocalization::get(_category->getName()));
    }

    OPCocosThread::cancelInvoke(_updateIndicatorInvoke);

    auto appManager = OPAppManager::shared();
    const auto& items = appManager->getDatabase()->getCategories(_category);

    setItems(items);

    if (_indicatorList) {
        _indicatorList->setNumItems(items.size());
    }
}

void OPCategoryGroupListComponent::doSpecialEffect(fairygui::EventContext* context)
{

    OPCategoryListComponent::doSpecialEffect(context);
    int firstItemIndex = _itemsList->getFirstChildInView();
    if (firstItemIndex == _items.size() - 1) {
        _currentIndex = 0;
    } else {
        _currentIndex = firstItemIndex + 1;
    }
    if (_indicatorList){
        OPCocosThread::cancelInvoke(_updateIndicatorInvoke);
        _updateIndicatorInvoke = OPCocosThread::invoke("update indicator", [this] {
          _indicatorList->setNumItems(_itemsList->getNumItems());
        });
    }
}

OPCategoryGroupListComponent::~OPCategoryGroupListComponent()
{
    OPCocosThread::cancelInvoke(_updateIndicatorInvoke);
}

void OPFeaturedCategoryGroupListComponent::onConstruct()
{
    OPCategoryGroupListComponent::onConstruct();
    if (Opalib::shared()->isTabletResolution()) {
        _columnGap = 160;
        _itemWidthProportion = 0.35f;
    }

    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    _featuredItemWidth = winSize.width * _itemWidthProportion;
    _featuredItemHeight = _featuredItemWidth / 1.78f;
    _itemsList->setColumnGap(_columnGap);
    _itemsList->setWidth((_featuredItemWidth * 3) + _itemsList->getColumnGap() * 2);
//    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() {
//    _itemsList->refreshVirtualList();
//    });
}

void OPFeaturedCategoryGroupListComponent::renderListItem(int index, fairygui::GObject* obj)
{
    OPCategoryGroupListComponent::renderListItem(index, obj);
    obj->setSize(_featuredItemWidth, _featuredItemHeight);
}
