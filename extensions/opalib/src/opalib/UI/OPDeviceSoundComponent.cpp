//
// Created by Talles Borges  on 16/07/20.
//

#include "OPDeviceSoundComponent.hpp"

void OPDeviceSoundComponent::onConstruct()
{
    OPFairyComponent::onConstruct();

    auto params = (OPDeviceSoundComponentParams*)_params.get();

    _nameTextField = getChild(params->nameId)->as<fairygui::GTextField>();

    _playButton = getChild(params->playButtonId)->as<fairygui::GButton>();
    _playButton->addClickListener([this](fairygui::EventContext* context) {
        _playItemCallback(_item);
        context->stopPropagation();
    });
}

void OPDeviceSoundComponent::setItem(OPDeviceSound* item)
{
    _item = item;

    _nameTextField->setText(_item->getName());
}

OPDeviceSound* OPDeviceSoundComponent::getItem() const
{
    return _item;
}

SL_TimeTrackerPercentCallback& OPDeviceSoundComponent::getTimeTrackerPercentCallback()
{
	return _timeTrackerPercentCallback;
}

void OPDeviceSoundComponent::setPlayItemCallback(const SL_PlayItemCallback& playItemCallback)
{
    _playItemCallback = playItemCallback;
}

void OPDeviceSoundComponent::setTimeTrackerPercentCallback(const SL_TimeTrackerPercentCallback& timeTrackerPercentCallback)
{
	_timeTrackerPercentCallback = timeTrackerPercentCallback;
}

void OPDeviceSoundComponent::setPlayingAnimation(bool isPlaying)
{
    auto params = (OPDeviceSoundComponentParams*)_params.get();

	auto controller = getPlayingController();
	
    controller->setSelectedPage(isPlaying ? params->playingId : params->stoppedId);
}

fairygui::GController* OPDeviceSoundComponent::getPlayingController() {
	auto params = (OPDeviceSoundComponentParams*)_params.get();
	return _playButton->getController(params->playControllerId);
}
