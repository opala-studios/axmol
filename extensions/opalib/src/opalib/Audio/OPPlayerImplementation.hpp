//
// Created by Ricardo on 14/10/21.
//

#ifndef OPPLAYERIMPLEMENTATION_HPP
#define OPPLAYERIMPLEMENTATION_HPP

#include <opalib/Audio/OPAudioBuffer.hpp>

class OPPlayerImplementation {
public:
    virtual ~OPPlayerImplementation() = default;

    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    
    virtual bool isReady() const = 0;

    virtual int getLengthInSamples() const = 0;
    virtual double getLengthInSeconds() const = 0;
    virtual bool isPlaying() const = 0;

    virtual void setSpeed(double speed) = 0;
    virtual double getSpeed() const = 0;

    virtual void setLooping (bool shouldLoop) = 0;
    virtual bool isLooping () const = 0;

    virtual void setVolume(float) = 0;
    virtual float getVolume() const = 0;

    virtual void setCurrentPositionInSamples(int) = 0;
    virtual int getCurrentPositionInSamples() const = 0;

    virtual double getSourceSampleRate() = 0;
    virtual std::shared_ptr<OPAudioBuffer> getAudioBuffer() { return nullptr; }
};

#endif //OPPLAYERIMPLEMENTATION_HPP
