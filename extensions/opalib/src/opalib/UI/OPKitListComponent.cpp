//
// Created by Talles Borges  on 04/06/20.
//

#include "OPKitListComponent.hpp"

#include <utility>

#include "../Misc/OPEventBus.hpp"

void OPKitListComponent::onConstruct()
{
    OPFairyComponent::onConstruct();

    linkComponents();
}

void OPKitListComponent::linkComponents()
{
    auto params = getParams<OPKitListComponentParams>();

    _customRenderItem = params->customRenderItem;
    _customSelectedKitHandler = params->customKitSelectedHandler;

    _itemsList = getChild(params->listId)->as<fairygui::GList>();
    if (params->isVirtual) {
        _itemsList->setVirtual();
    }

    _itemsList->itemRenderer = CC_CALLBACK_2(OPKitListComponent::renderListItem, this);
    _itemsList->addEventListener(fairygui::UIEventType::ClickItem, [this](fairygui::EventContext* context) {
        int childIndex = _itemsList->getChildIndex((GObject*)context->getData());
        auto kitIndex = _itemsList->childIndexToItemIndex(childIndex);
        auto kit = _items.at(kitIndex);

        if (_customSelectedKitHandler) {
            _customSelectedKitHandler(kit, this);
            return;
        }

        auto params = getParams<OPKitListComponentParams>();

        auto event = opalib::Event::KitSelected();
        event.kit = kit;
        event.categoryName = params->categoryName;
        event.location = params->locationName;

        OPEventBus::shared()->notify(event);
    });

    if (!params->textFieldId.empty()) {
        _textField = getChild(params->textFieldId)->as<fairygui::GTextField>();
        if (params->onTextFieldChange) {
            _textField->addEventListener(fairygui::UIEventType::Changed, [this](fairygui::EventContext*) {
                auto params = getParams<OPKitListComponentParams>();
                params->onTextFieldChange(_textField, this);
            });
        }
    }
}

void OPKitListComponent::setItems(cocos2d::Vector<OPKit*> items)
{
    _items = std::move(items);
    refreshList();
}

void OPKitListComponent::setTitle(const std::string& title)
{
    _textField->setText(OPLocalization::get(title));
}

void OPKitListComponent::appendItems(const cocos2d::Vector<OPKit*>& items)
{
    _items.pushBack(items);
    refreshList();
}

void OPKitListComponent::renderListItem(int index, fairygui::GObject* obj)
{
    if (index >= _items.size()) {
        return;
    }
    auto kit = _items.at(index);

    auto item = obj->as<OPKitComponent>();
    if (item) {
        item->setKit(kit);
        if (_customRenderItem) {
            _customRenderItem(this, item, kit, index);
        }
    }
}

const cocos2d::Vector<OPKit*>& OPKitListComponent::getItems()
{
    return _items;
}
const std::string& OPKitListComponent::getText()
{
    return _textField->getText();
}

void OPKitListComponent::refreshList()
{
    auto params = getParams<OPKitListComponentParams>();
    if (params->isVirtual) {
        _itemsList->setNumItems((int)_items.size());
    } else {
        _itemsList->setNumItems(params->nonVirtualItems);
    }
}

void OPKitListComponent::setLocationName(const std::string& name)
{
    getParams<OPKitListComponentParams>()->locationName = name;
}

void OPKitListComponent::setCustomSelectedKitHandler(const CustomKitSelectedHandler& customSelectedKitHandler)
{
    _customSelectedKitHandler = customSelectedKitHandler;
}
