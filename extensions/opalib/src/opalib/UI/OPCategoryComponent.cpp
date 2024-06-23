//
// Created by Talles Borges  on 05/06/20.
//

#include "OPCategoryComponent.hpp"

void OPCategoryComponent::onConstruct()
{
  	OPFairyComponent::onConstruct();

    auto params = getParams<OPCategoryComponentParams>();

    if (!params->loaderId.empty()) {
        _imageLoader = getChildByPath(params->loaderId)->as<fairygui::GLoader>();
    }

    if (!params->titleId.empty()) {
        _titleTextField = getChild(params->titleId)->as<fairygui::GTextField>();
    }
}

void OPCategoryComponent::setCategory(OPCategory* category)
{
    _category = category;

    setImage();

    if (_titleTextField) {
        _titleTextField->setText(category->getName());
    }
}
void OPCategoryComponent::setImage()
{
    if (_imageLoader) {
        if (_category->getType() == OPCategory::FEATURED_ITEM || _category->getType() == OPCategory::FEATURED_URL
            || _category->getType() == OPCategory::FEATURED_CATEGORY) {
            _imageLoader->setURL(_category->getURLBackground());
        } else {
            _imageLoader->setURL(_category->getURLThumb());
        }
    }
}

OPCategory* OPCategoryComponent::getCategory() const
{
    return _category;
}

void OPCategoryPlaceholderComponent::onConstruct()
{
    OPCategoryComponent::onConstruct();

    auto params = getParams<OPCategoryPlaceholderComponentParams>();

    _imageLoader = getChild(params->placeholderId)->as<fairygui::GComponent>()->getChild(params->loaderId)->as<fairygui::GLoader>();
}

void OPCategoryPlaceholderComponent::setImage()
{
    OPCategoryComponent::setImage();
    auto params = getParams<OPCategoryPlaceholderComponentParams>();
    if (!params->controllerId.empty()) {
        getChild(params->placeholderId)->as<fairygui::GComponent>()->getController(params->controllerId)->setSelectedIndex(1);
    }
}
