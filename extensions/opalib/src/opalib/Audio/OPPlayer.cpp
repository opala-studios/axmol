//
//  OPPlayer.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 08/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//
#include "../Application/OPApplication.hpp"
#include "OPPlayer.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include <Debug/OPDebug.hpp>
#include <Misc/OPCocosThread.hpp>
#include <utility>

OPPlayer::OPPlayer(){
    OP_LOG("BEGIN");
    _listener.listen<OPAudioEngine::EventAudioPause>([this](const OPAudioEngine::EventAudioPause& ev){
        onAudioEnginePaused(ev);
    });
    
    _listener.listen<OPAudioEngine::EventAudioResume>([this](const OPAudioEngine::EventAudioResume& ev){
        onAudioEngineResume(ev);
    });
    
    _listener.listen<OPAudioEngine::EventCreateImplementation>([this](const OPAudioEngine::EventCreateImplementation& ev){
        createImplementation();
    });
    
    _listener.listen<OPAudioEngine::EventDestroyImplementation>([this](const OPAudioEngine::EventDestroyImplementation& ev){
        destroyImplementation();
    });
    OP_LOG("END");
}


void OPPlayer::play()
{
    OP_LOG("PLAY");
    if(isPlaying())
        return;
    
    _isFinished = false;

    _implementation->play();
}


void OPPlayer::pause()
{
    OP_LOG("PAUSE");
    if (!isPlaying()) { return; }

    _implementation->pause();

}

void OPPlayer::stop()
{
    OP_LOG("STOP");
    if (!isPlaying())
    {
        _implementation->setCurrentPositionInSamples(0);
        return;
    }
    
    if(_stopTrackCallback)
        _stopTrackCallback(_isFinished);

    _implementation->stop();
    _implementation->setCurrentPositionInSamples(0);
}


void OPPlayer::resume()
{
    OP_LOG("RESUME");
    _implementation->resume();
}


double OPPlayer::getLengthInSeconds() const
{
    return _implementation->getLengthInSeconds();
}


bool OPPlayer::isPlaying() const
{
    return isReady() && _implementation->isPlaying();
}

void OPPlayer::setSpeed(double speed)
{
    _implementation->setSpeed(speed);
}

double OPPlayer::getSpeed()
{
    return _implementation->getSpeed();
}


void OPPlayer::setLooping(bool shouldLoop)
{
    _implementation->setLooping(shouldLoop);
}


bool OPPlayer::isLooping() const
{
    return _implementation->isLooping();
}


void OPPlayer::setVolume(float volume)
{
    //clamping value
    volume = juce::jmax(0.0f, juce::jmin(1.0f, volume));


    _implementation->setVolume(volume);
}


float OPPlayer::getVolume() {
    return _implementation->getVolume();
}


void OPPlayer::setCurrentPositionPercent(float percent)
{
    _implementation->setCurrentPositionInSamples(_implementation->getLengthInSamples() * percent);
}


void OPPlayer::setCurrentPositionInSamples(int position)
{
    _implementation->setCurrentPositionInSamples(position);
}


void OPPlayer::setCurrentPositionInSeconds(float seconds)
{
    auto deviceSampleRate =_implementation->getSourceSampleRate();
    _implementation->setCurrentPositionInSamples(deviceSampleRate * seconds);
}


float OPPlayer::getCurrentPositionPercent()
{
    return float(_implementation->getCurrentPositionInSamples())/float(_implementation->getLengthInSamples());
}

float OPPlayer::getCurrentPositionInSeconds()
{
    return getCurrentPositionInSamples() / _implementation->getSourceSampleRate();;
}

bool OPPlayer::isReady() const {
    return _implementation && _implementation->isReady();
}

int OPPlayer::getCurrentPositionInSamples()
{
    return _implementation->getCurrentPositionInSamples();
}

void OPPlayer::setStopTrackCallback(StopTrackCallback callback)
{
    _stopTrackCallback = std::move(callback);
}

void OPPlayer::setTimeTrackCallback(TimeTrackCallback callback)
{
    _timeTrackCallback = std::move(callback);
    resumeTimer();
}

void OPPlayer::setTimeTrackPercentCallback(TimeTrackPercentCallback callback)
{
    _timeTrackPercentCallback = std::move(callback);
    resumeTimer();
}

void OPPlayer::update(float dt)
{
    if (_timeTrackCallback) {
        _timeTrackCallback(getCurrentPositionInSamples() / getSourceSampleRate());
    }

    if (_timeTrackPercentCallback) {
        _timeTrackPercentCallback(getCurrentPositionPercent());
    }
}

void OPPlayer::resumeTimer()
{
    OP_LOG("BEGIN");
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this,0,false);
    OP_LOG("END");
}

void OPPlayer::stopTimer()
{
    OP_LOG("BEGIN");
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    OP_LOG("END");
}

void OPPlayer::onAudioEnginePaused(const OPAudioEngine::EventAudioPause &ev) {
    OP_LOG("BEGIN");
    _positionOnPause = _implementation->getCurrentPositionInSamples();
    OP_LOG("END");
}

void OPPlayer::onAudioEngineResume(const OPAudioEngine::EventAudioResume &ev) {
    OP_LOG("BEGIN");
    setCurrentPositionInSamples(_positionOnPause);
    OP_LOG("END");
}

OPPlayer::~OPPlayer()
{
    OP_LOG("BEGIN");
    stopTimer();
    OP_LOG("END");
}

double OPPlayer::getSourceSampleRate() {
    return _implementation->getSourceSampleRate();
}
