//
// Created by Talles Borges  on 05/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITCOMPONENT_HPP_

#include "../Models/OPKit.hpp"
#include "OPFairyComponent.hpp"

struct OPKitComponentParams : public OPFairyComponentParams
{
    std::string nameId;
    std::string musicId;
    std::string authorId;
};

class OPKitComponent : public OPFairyComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPKitComponent)

protected:
    void onConstruct() override;

public:
    virtual void setKit(OPKit* kit);
    virtual OPKit* getKit();
protected:
    OPKit* _kit;

    fairygui::GTextField* _nameTextField;
    fairygui::GTextField* _musicNameTextField;
    fairygui::GTextField* _authorNameTextField;
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPKITCOMPONENT_HPP_
