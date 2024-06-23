//
// Created by Talles Borges  on 16/07/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDCOMPONENT_HPP_

#include "./OPFairyComponent.hpp"
#include "../Native/OPDeviceSound.hpp"

typedef std::function<void(OPDeviceSound* item)> SL_PlayItemCallback;
typedef std::function<void(float percent)> SL_TimeTrackerPercentCallback;

struct OPDeviceSoundComponentParams : OPFairyComponentParams
{
    std::string nameId;

    std::string playButtonId;
    std::string playControllerId;
    std::string playingId;
    std::string stoppedId;
};

class OPDeviceSoundComponent : public OPFairyComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPDeviceSoundComponent)

    virtual void setItem(OPDeviceSound* item);
    OPDeviceSound* getItem() const;

    void setPlayItemCallback(const SL_PlayItemCallback& playItemCallback);
	void setTimeTrackerPercentCallback(const SL_TimeTrackerPercentCallback& timeTrackerPercentCallback);
    void setPlayingAnimation(bool playing);
	
	SL_TimeTrackerPercentCallback& getTimeTrackerPercentCallback();

protected:
    void onConstruct() override;
	
	virtual fairygui::GController* getPlayingController();

    fairygui::GButton* _playButton;
    fairygui::GTextField* _nameTextField;

    SL_PlayItemCallback _playItemCallback;
	SL_TimeTrackerPercentCallback _timeTrackerPercentCallback;

    OPDeviceSound* _item;
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDCOMPONENT_HPP_
