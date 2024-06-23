//
// Created by Talles Borges  on 04/06/20.
//

#include "OPCategoryListComponent.hpp"

#include <utility>

#include "../Managers/OPAppManager.hpp"
#include "../Misc/OPEventBus.hpp"

void OPCategoryListComponent::onConstruct()
{
    OPCategoryComponent::onConstruct();

    auto params = getParams<OPCategoryListComponentParams>();

    _itemsList = getChild(params->listId)->as<fairygui::GList>();
    _itemsList->setVirtualAndLoop();
    _itemsList->itemRenderer = CC_CALLBACK_2(OPCategoryListComponent::renderListItem, this);

    if (params->shouldZoomItems) {
        _itemsList->addEventListener(fairygui::UIEventType::Scroll, CC_CALLBACK_1(OPCategoryListComponent::doSpecialEffect, this));
        _itemsList->addEventListener(fairygui::UIEventType::ScrollEnd, CC_CALLBACK_1(OPCategoryListComponent::doSpecialEffect, this));
    }

    _itemsList->addEventListener(fairygui::UIEventType::ClickItem, [this](fairygui::EventContext* context) {
        int childIndex = _itemsList->getChildIndex((GObject*)context->getData());
        auto kitIndex = _itemsList->childIndexToItemIndex(childIndex);
        auto category = _items.at(kitIndex);

        auto reference = category->getReferencedId();
        switch (category->getType()) {
            case OPCategory::FEATURED_ITEM: {
                auto appManager = OPAppManager::shared();

                auto event = opalib::Event::KitSelected();
                event.kit = appManager->getDatabase()->getKit(reference);
                event.categoryName = category->getName();
                event.location = "FEATURED_KIT";

                OPEventBus::shared()->notify(event);
                break;
            }
            case OPCategory::FEATURED_CATEGORY: {
                auto appManager = OPAppManager::shared();

                auto event = opalib::Event::CategorySelected();
                event.category = appManager->getDatabase()->getCategory(reference);

                OPEventBus::shared()->notify(event);
                break;
            }
            case OPCategory::FEATURED_URL: {
                cocos2d::Application::getInstance()->openURL(reference);
                break;
            }
            default: {
                auto event = opalib::Event::CategorySelected();
                event.category = category;

                OPEventBus::shared()->notify(event);
                break;
            }
        }
    });
}

void OPCategoryListComponent::renderListItem(int index, fairygui::GObject* obj)
{
    auto category = _items.at(index);

    auto item = obj->as<OPCategoryComponent>();
    if (item) {
        item->setCategory(category);
    }
}

void OPCategoryListComponent::setItems(const cocos2d::Vector<OPCategory*>& items)
{
    _items = items;
    _itemsList->setNumItems((int)_items.size());

    auto params = getParams<OPCategoryListComponentParams>();

    if (params->shouldZoomItems) {
        _itemsList->scrollToView(_items.size() - 1, false, true);
    }
}

void OPCategoryListComponent::doSpecialEffect(fairygui::EventContext* context)
{
    // change the scale according to the distance to middle
    float midX = _itemsList->getScrollPane()->getPosX() + _itemsList->getViewWidth() / 2;
    int cnt = _itemsList->numChildren();
    for (int i = 0; i < cnt; i++) {
        GObject* obj = _itemsList->getChildAt(i);
        float dist = std::abs(midX - obj->getX() - obj->getWidth() / 2);
        if (dist > obj->getWidth()) // no intersection
            obj->setScale(1, 1);
        else {
            float ss = 1 + (1 - dist / obj->getWidth()) * 0.24f;
            obj->setScale(ss, ss);
        }
    }
}
