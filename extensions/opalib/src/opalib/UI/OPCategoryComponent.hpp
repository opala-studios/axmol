//
// Created by Talles Borges  on 05/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYCOMPONENT_HPP_

#include "OPFairyComponent.hpp"
#include "../Models/OPCategory.hpp"

struct OPCategoryComponentParams : public virtual OPFairyComponentParams
{
    std::string loaderId;
    std::string titleId;
};

class OPCategoryComponent : public virtual OPFairyComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPCategoryComponent);

    virtual void setCategory(OPCategory* category);
    OPCategory* getCategory() const;

protected:
    void onConstruct() override;
    virtual void setImage();

    fairygui::GLoader* _imageLoader;
    fairygui::GTextField* _titleTextField;

    OPCategory* _category;
};

struct OPCategoryPlaceholderComponentParams : public OPCategoryComponentParams
{
    std::string placeholderId;
    std::string controllerId;
};

//TODO: Check if this class is needed, maybe a refactor on the OPCategoryComponent should works
class OPCategoryPlaceholderComponent : public OPCategoryComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPCategoryPlaceholderComponent)
protected:
    void onConstruct() override;
    void setImage() override;
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYCOMPONENT_HPP_
