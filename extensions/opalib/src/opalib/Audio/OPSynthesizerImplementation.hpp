//
// Created by Ricardo on 13/10/21.
//

#ifndef OPSYNTHESIZERIMPLEMENTATION_HPP
#define OPSYNTHESIZERIMPLEMENTATION_HPP

#include <opalib/Audio/OPSynthItem.hpp>
#include <functional>

class OPSynthesizerImplementation {
public:
    typedef std::function<void(bool)> LoadItemHandler;
public:
    virtual ~OPSynthesizerImplementation() = default;
    virtual void play(OPSynthItem* item) = 0;
    virtual void stop(OPSynthItem* item) = 0;
    virtual void loadItem(OPSynthItem* item,const LoadItemHandler& handler) = 0;
    virtual void unloadItem(OPSynthItem* item) = 0;
    virtual void unloadAll() = 0;

    virtual bool isLoaded(OPSynthItem* item) const = 0;

    virtual void setIsLooping(OPSynthItem* item, bool looping) = 0;
    virtual bool getIsLooping(OPSynthItem* item) const = 0;
    
    virtual void setSpeed(float speed) = 0;
    virtual void setSpeed(OPSynthItem* item, float speed) = 0;
    virtual float getSpeed(OPSynthItem* item) const = 0;
    
    virtual void setVolume(float volume) = 0;
    virtual void setVolume(OPSynthItem* item, float volume) = 0;
    virtual float getVolume(OPSynthItem* item) const = 0;
};

#endif //OPSYNTHESIZERIMPLEMENTATION_HPP
