//
//  OPPlayer.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 08/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPPlayer_hpp
#define OPPlayer_hpp

#include <stdio.h>
#include <JuceHeader.h>
#include <eventbus/TokenHolder.h>
#include "OPAudioBuffer.hpp"
#include "OPMemoryAudioSource.hpp"
#include "Application/OPApplication.hpp"
#include "OPPlayerImplementation.hpp"

class OPPlayer
{
public:
    typedef std::function<void(bool isFinished)> StopTrackCallback;
    typedef std::function<void(float)> TimeTrackCallback;
    typedef std::function<void(float)> TimeTrackPercentCallback;
    
    OPPlayer();
    virtual ~OPPlayer();
    
    virtual void play();
    virtual void pause();
    virtual void stop();
    virtual void resume();
    
    virtual double getLengthInSeconds() const;
    virtual bool isPlaying() const;
            
    virtual void setSpeed(double speed);
    virtual double getSpeed();
    
    virtual void setLooping (bool shouldLoop);
    virtual bool isLooping () const;
    
    virtual void setVolume(float);
    virtual float getVolume();
    
    virtual void setCurrentPositionPercent(float);
    virtual void setCurrentPositionInSamples(int);
    virtual void setCurrentPositionInSeconds(float);
    
    virtual float getCurrentPositionPercent();
    virtual float getCurrentPositionInSeconds();
    virtual int getCurrentPositionInSamples();
    
    virtual void setStopTrackCallback(StopTrackCallback);
    virtual void setTimeTrackCallback(TimeTrackCallback);
    virtual void setTimeTrackPercentCallback(TimeTrackPercentCallback);

    virtual double getSourceSampleRate();

    virtual void update(float dt);
    
    bool isReady() const;

    void resumeTimer();
    void stopTimer();
    
protected:
    
    virtual void createImplementation() = 0;
    virtual void destroyImplementation() = 0;
    
protected:

    std::unique_ptr<OPPlayerImplementation> _implementation;
    int _positionOnPause = 0.0f;
    bool _isFinished = false;
    
private:
    StopTrackCallback _stopTrackCallback;
    TimeTrackCallback _timeTrackCallback;
    TimeTrackPercentCallback _timeTrackPercentCallback;
    Dexode::TokenHolder<Dexode::EventBus> _listener {OPApplication::shared()->getAudioEngine()->getEventBus()};

    void onAudioEnginePaused(const OPAudioEngine::EventAudioPause& ev);
    void onAudioEngineResume(const OPAudioEngine::EventAudioResume& ev);

};

#endif /* OPPlayer_hpp */
