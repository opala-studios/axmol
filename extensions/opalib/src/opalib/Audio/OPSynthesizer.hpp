//
// Created by Talles Borges  on 09/11/20.
//

#ifndef OPSYNTHESIZER_HPP
#define OPSYNTHESIZER_HPP

#include <opalib/Audio/OPSynthItem.hpp>
#include <opalib/Audio/OPSynthesizerImplementation.hpp>
#include <opalib/Application/OPApplication.hpp>
#include "eventbus/EventBus.h"
#include "eventbus/TokenHolder.h"

class OPSynthesizer
{

public:
    typedef std::function<void(bool)> LoadItemHandler;

    OPSynthesizer();
    ~OPSynthesizer();
    void play(OPSynthItem* item);
    void stop(OPSynthItem* item);
    void loadItem(OPSynthItem* item,const LoadItemHandler& handler);
    void unloadItem(OPSynthItem* item);

    void unloadAllSounds();

    void setSpeed(float speed);
    void setSpeed(OPSynthItem* item, float speed);
    float getSpeed(OPSynthItem* item) const;

    bool isLoaded(OPSynthItem* item) const;
    
    void setIsLooping(OPSynthItem* item, bool looping);
    bool getIsLooping(OPSynthItem* item) const;
    
    void setVolume(float volume);
    void setVolume(OPSynthItem* item, float volume);
    float getVolume(OPSynthItem* item) const;
private:
    void createImplementation();
    void destroyImplementation();

private:
    std::map<int, OPSynthItem *> _lastPlayedItemByGroup;
    std::set<OPSynthItem*> _ownedItems;
    Dexode::TokenHolder<Dexode::EventBus> _listener{OPApplication::shared()->getAudioEngine()->getEventBus()};
    std::unique_ptr<OPSynthesizerImplementation> _implementation;

};

#endif // OPSYNTHESIZER_HPP
