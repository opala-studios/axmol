//
// Created by Talles Borges  on 10/06/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPFAIRYCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPFAIRYCOMPONENT_HPP_

#include <FairyGUI.h>
#include <cocos2d.h>

#include "../Localization/OPLocalization.hpp"
#include "../Misc/OPMacros.hpp"

typedef std::function<void(fairygui::GComponent*)> SEL_ComponentHandler;
typedef std::function<void()> SEL_ComponentBackActionHandler;

struct OPFairyComponentParams
{
    std::vector<std::pair<std::string, std::function<void(fairygui::EventContext*)>>> clickListeners;
    SEL_ComponentHandler onConstruct = nullptr;
    SEL_ComponentHandler onEnter = nullptr;
    SEL_ComponentBackActionHandler onBackPressed = nullptr;
    std::vector<TranslatableItem> translations;

    virtual ~OPFairyComponentParams(){};
};

class OPFairyComponent : public fairygui::GComponent
{
public:
    CREATE_FUNC(OPFairyComponent)
    CREATE_FUNC_WITH_ARGS(OPFairyComponent);

    void setControllerValue(const std::string& controller, const std::string& value);
    void setControllerIndex(const std::string& controller, int index);

    void setOnEnterListener(const SEL_ComponentHandler& onEnterListener);

    virtual void onBackPressed();

protected:
    using fairygui::GComponent::init; // unhide the base method, the method init with params make it hidden
    virtual bool init(std::unique_ptr<OPFairyComponentParams> params);

    void onConstruct() override;
    void onEnter() override;

    template <typename T>
    T* getParams()
    {
        return dynamic_cast<T*>(_params.get());
    };

protected:
    std::unique_ptr<OPFairyComponentParams> _params;
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_MENU_OPFAIRYCOMPONENT_HPP_
