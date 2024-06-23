//
// Created by Talles Borges  on 04/06/20.
//

#include "OPCategoryKitListComponent.hpp"

#include "../Managers/OPAppManager.hpp"

void OPCategoryKitListComponent::setCategory(OPCategory* category)
{
    OPCategoryComponent::setCategory(category);

    getParams<OPKitListComponentParams>()->categoryName = category->getName();
    setTitle(category->getName());

    auto appManager = OPAppManager::shared();
    setItems(appManager->getDatabase()->getKits(_category));
}

void OPCategoryKitListComponent::onConstruct()
{
    OPCategoryComponent::onConstruct();
    OPKitListComponent::linkComponents();
}
