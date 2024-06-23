//
// Created by Talles Borges  on 05/06/20.
//

#include "OPKitComponent.hpp"
#include "../Helpers/OPString.hpp"

void OPKitComponent::onConstruct()
{
    OPFairyComponent::onConstruct();

    auto params = (OPKitComponentParams*)_params.get();

    if (!params->musicId.empty()) {
        _musicNameTextField = getChildByPath(params->musicId)->as<fairygui::GTextField>();
    }
    if (!params->authorId.empty()) {
        _authorNameTextField = getChildByPath(params->authorId)->as<fairygui::GTextField>();
    }

    if (!params->nameId.empty()) {
        _nameTextField = getChildByPath(params->nameId)->as<fairygui::GTextField>();
    }
}

void OPKitComponent::setKit(OPKit* kit)
{
    _kit = kit;

    if (_nameTextField) {
        auto text = OPString::uppercased(_kit->getName());
        if (_nameTextField->getText() != text) {
            _nameTextField->setText(text);    
        }
    }

    if (_musicNameTextField) {
        auto text = _kit->getMusicName();
        if (_musicNameTextField->getText() != text) {
            _musicNameTextField->setText(text);       
        }
    }

    if (_authorNameTextField) {
        auto text = _kit->getAuthorName();
        if (_authorNameTextField->getText() != text) {
            _authorNameTextField->setText(text);
        }
    }
}

OPKit* OPKitComponent::getKit()
{
    return _kit;
}
