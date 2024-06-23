//
// Created by Talles Borges  on 16/07/20.
//

#include <opalib/Audio/OPMemoryPlayer.hpp>
#include <opalib/Audio/OPStreamedPlayer.hpp>
#include <opalib/Debug/OPDebug.hpp>
#include "OPDeviceSoundListComponent.hpp"

#include "OPDeviceSoundComponent.hpp"

void OPDeviceSoundListComponent::onConstruct()
{
    OPFairyComponent::onConstruct();

    auto params = (OPDeviceSoundListComponentParams*)_params.get();

    _itemsList = getChild(params->listId)->as<fairygui::GList>();
    _itemsList->itemRenderer = CC_CALLBACK_2(OPDeviceSoundListComponent::renderListItem, this);
    _itemsList->setVirtual();

    _itemClickListener = params->itemClickListener;
    _onItemPlayed = params->onItemPlayed;

    _itemsList->addEventListener(fairygui::UIEventType::ClickItem, [this](fairygui::EventContext* context) {
        OP_LOG("Begin - Item clicked");
        stopPlayer();
        if (_itemClickListener) {
            int childIndex = _itemsList->getChildIndex((GObject*)context->getData());
            auto index = _itemsList->childIndexToItemIndex(childIndex);
            auto item = _items.at(index);

            _itemClickListener(item);
        }
        OP_LOG("End - Item clicked");
    });
}

void OPDeviceSoundListComponent::setItems(const cocos2d::Vector<OPDeviceSound*>& items)
{
    _items = items;
    updateList();
}

void OPDeviceSoundListComponent::setItemClickListener(const std::function<void(OPDeviceSound*)>& itemClickListener)
{
    _itemClickListener = itemClickListener;
}

void OPDeviceSoundListComponent::renderListItem(int index, fairygui::GObject* obj)
{
    auto item = _items.at(index);

    auto component = obj->as<OPDeviceSoundComponent>();
    if (component) {
        component->setItem(item);
        component->setPlayingAnimation(index == _playingIndex);
        component->setPlayItemCallback([this, component](OPDeviceSound* item) {
            auto recordingIndex = (int)_items.getIndex(item);
            // handling audio
            if (recordingIndex == _playingIndex) {
                stopPlayer();
                _playingIndex = -1;
            } else {
                playItem(recordingIndex);
                _playingIndex = recordingIndex;
            }
			
			_soundPlayer->setTimeTrackPercentCallback([this, component](float percent) {
				if(percent <= 1.0) {
					auto callback = component->getTimeTrackerPercentCallback();
					
					if(callback)
						callback(percent);
				} else {
					component->setPlayingAnimation(false);
					stopPlayer();
					_playingIndex = -1;
				}
			});
			
            updateList();
        });
        onRenderItem(index, component);
    }
}

void OPDeviceSoundListComponent::playItem(int index)
{
    OP_LOG("BEGIN");
    if (_soundPlayer) {
        _soundPlayer->stop();
    }

    auto recording = _items.at(index);
    _playingIndex = index;

    OP_LOG("Play: " + recording->getFilePath());
    _soundPlayer = OPStreamedPlayer::createUnique(recording->getFilePath());

    _soundPlayer->play();

    _soundPlayer->setStopTrackCallback([this](bool isFinished) {
        _playingIndex = -1;
        updateList();
    });

    if (_onItemPlayed) {
        _onItemPlayed();
    }

    OP_LOG("END");
}

void OPDeviceSoundListComponent::stopPlayer()
{
    if (!_soundPlayer) return;
    if (!_soundPlayer->isPlaying()) return;

    _soundPlayer->stop();
}

void OPDeviceSoundListComponent::updateList()
{
    _itemsList->setNumItems((int)_items.size());
}

void OPDeviceSoundListComponent::onEnter()
{
    OPFairyComponent::onEnter();
    stopPlayer();
}


void OPDeviceSoundListComponent::onExit()
{
    stopPlayer();
    GComponent::onExit();
}

OPDeviceSoundListComponent::~OPDeviceSoundListComponent()
{

}

void OPDeviceSoundListComponent::setAudioSource(const std::string &audioSource)
{
    _audioSource = audioSource;
}
