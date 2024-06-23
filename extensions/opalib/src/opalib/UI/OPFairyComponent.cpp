//
// Created by Talles Borges  on 10/06/20.
//

#include "OPFairyComponent.hpp"

#include "../Helpers/OPString.hpp"

bool OPFairyComponent::init(std::unique_ptr<OPFairyComponentParams> params)
{
    if (!fairygui::GComponent::init()) {
        return false;
    }

    _params = std::move(params);

    return true;
}

void OPFairyComponent::onConstruct()
{
    GComponent::onConstruct();

    if (!_params) {
        return;
    }

    for (const auto& item : _params->clickListeners) {
        getChildByPath(item.first)->addClickListener([item](fairygui::EventContext* context) {
            item.second(context);
        });
    }

    for (const auto& item : _params->translations) {
        std::string localizedText;
        if (item.args.empty()) localizedText = OPLocalization::get(item.localizationKey);
        else
            localizedText = OPLocalization::get(item.localizationKey, item.args);

        if (item.uppercased) localizedText = OPString::uppercased(localizedText);

        getChildByPath(item.childPath)->setText(localizedText);
    }

    if (_params->onConstruct) {
        _params->onConstruct(this);
    }
}

void OPFairyComponent::onEnter()
{
    GComponent::onEnter();

    if (!_params) {
        return;
    }

    if (_params->onEnter) {
        _params->onEnter(this);
    }
}

void OPFairyComponent::setControllerValue(const std::string& controller, const std::string& value)
{
    getController(controller)->setSelectedPage(value);
}

void OPFairyComponent::setControllerIndex(const std::string& controller, int index)
{
    getController(controller)->setSelectedIndex(index);
}

void OPFairyComponent::setOnEnterListener(const SEL_ComponentHandler& listener)
{
    _params->onEnter = listener;
}

void OPFairyComponent::onBackPressed()
{
    if (!_params) {
        return;
    }

    if (_params->onBackPressed) {
        _params->onBackPressed();
    }
}
