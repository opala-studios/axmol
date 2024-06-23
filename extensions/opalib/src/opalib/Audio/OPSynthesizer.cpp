//
// Created by Talles Borges  on 09/11/20.
//

#include "../Application/OPApplication.hpp"
#include "OPSynthesizer.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "OPSampler.h"
#include "../Helpers/OPJuceHelper.hpp"
#include "../Debug/OPDebug.hpp"

OPSynthesizer::OPSynthesizer() {

    _listener.listen<OPAudioEngine::EventCreateImplementation>([this](const OPAudioEngine::EventCreateImplementation& ev){
        createImplementation();
    });

    _listener.listen<OPAudioEngine::EventDestroyImplementation>([this](const OPAudioEngine::EventDestroyImplementation& ev){
        destroyImplementation();
    });
    createImplementation();
}

void OPSynthesizer::play(OPSynthItem* item)
{
    _implementation->play(item);

    if(item->shouldHandleGroup()){
        if(_lastPlayedItemByGroup.count(item->getGroup()))
        {
            auto lastItem = _lastPlayedItemByGroup.at(item->getGroup());

            if(item != lastItem)
            {
                stop(_lastPlayedItemByGroup.at(item->getGroup()));
                _lastPlayedItemByGroup.erase(item->getGroup());
            }
        }

        _lastPlayedItemByGroup.insert(std::pair<int, OPSynthItem*>(item->getGroup(),item));
    }

}

void OPSynthesizer::stop(OPSynthItem* item)
{
    _implementation->stop(item);
}

void OPSynthesizer::loadItem(OPSynthItem* item, const OPSynthesizer::LoadItemHandler& handler)
{
    _ownedItems.insert(item);
    _implementation->loadItem(item, [this, item, handler](bool success){
        if (!success){
            _ownedItems.erase(item);
        }
        if (handler){
            handler(success);
        }
    });
}

void OPSynthesizer::unloadItem(OPSynthItem* item) {
    stop(item);
    _ownedItems.erase(item);
    _implementation->unloadItem(item);
}

bool OPSynthesizer::isLoaded(OPSynthItem *item) const {
    return _implementation->isLoaded(item);
}

void OPSynthesizer::setIsLooping(OPSynthItem *item, bool looping){
    _implementation->setIsLooping(item, looping);
}

bool OPSynthesizer::getIsLooping(OPSynthItem *item) const {
    return _implementation->getIsLooping(item);
}

void OPSynthesizer::setSpeed(float speed) {
    _implementation->setSpeed(speed);
}

void OPSynthesizer::setSpeed(OPSynthItem* item, float speed){
    _implementation->setSpeed(item, speed);
}

float OPSynthesizer::getSpeed(OPSynthItem* item) const {
    return _implementation->getSpeed(item);
}

void OPSynthesizer::setVolume(float volume) {
    _implementation->setVolume(volume);
}

void OPSynthesizer::setVolume(OPSynthItem *item, float volume) {
    _implementation->setVolume(item, volume);
}

float OPSynthesizer::getVolume(OPSynthItem* item) const {
    return _implementation->getVolume(item);
}

void OPSynthesizer::unloadAllSounds() {
    _implementation->unloadAll();
}

OPSynthesizer::~OPSynthesizer() {
    OP_LOG("BEGIN");

    unloadAllSounds();

    OP_LOG("END");
}

void OPSynthesizer::createImplementation() {
    _implementation = std::unique_ptr<OPSynthesizerImplementation>(OPApplication::shared()->getAudioEngine()->createSynthesizerImplementation());

    for(auto item : _ownedItems){
        OPApplication::shared()->getAudioEngine()->incrementPendingImplementationCounter();
        loadItem(item, [](bool success){
            OPApplication::shared()->getAudioEngine()->decrementPendingImplementationCounter();
        });
    }
}

void OPSynthesizer::destroyImplementation() {
    _implementation.reset();
}

