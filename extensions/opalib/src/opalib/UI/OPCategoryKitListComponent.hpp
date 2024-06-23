//
// Created by Talles Borges  on 04/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYKITLISTCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYKITLISTCOMPONENT_HPP_

#include "OPKitListComponent.hpp"
#include "OPCategoryComponent.hpp"


struct OPCategoryKitListComponentParams : public OPCategoryComponentParams, public OPKitListComponentParams {

};

class OPCategoryKitListComponent : public OPCategoryComponent, public OPKitListComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPCategoryKitListComponent)
    void setCategory(OPCategory* category) override;

protected:
    void onConstruct() override;

};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPCATEGORYKITLISTCOMPONENT_HPP_
