//
// Created by Ricardo on 20/10/21.
//

#include "OPCricketStreamedPlayerImplementation.hpp"
#include <opalib/Debug/OPDebug.hpp>

OPCricketStreamedPlayerImplementation::OPCricketStreamedPlayerImplementation(
        const std::string &filepath) {
    OP_LOG("BEGIN");
    _sound = CkSound::newStreamSound(filepath.c_str());
    OP_LOG("END");
}

bool OPCricketStreamedPlayerImplementation::isReady() const {
    return _sound != nullptr;
}

void OPCricketStreamedPlayerImplementation::play() {
    if (_paused){
        resume();
        return;
    }
    _sound->play();
}

void OPCricketStreamedPlayerImplementation::stop() {
    _sound->stop();
}

void OPCricketStreamedPlayerImplementation::pause() {
    _lastPausePosition = getCurrentPositionInSamples();
    _sound->stop();
    _sound->setPlayPosition(_lastPausePosition);
    _paused = true;
}

void OPCricketStreamedPlayerImplementation::resume() {
    if (_sound->isPlaying()){
        return;
    }
    _sound->play();
    if (!_paused){
        return;
    }
//    _sound->setPlayPosition(_lastPausePosition);
    _paused = false;
}

int OPCricketStreamedPlayerImplementation::getLengthInSamples() const {
    return _sound->getLength();
}

double OPCricketStreamedPlayerImplementation::getLengthInSeconds() const {
    return _sound->getLengthMs() / 1000.0;
}

bool OPCricketStreamedPlayerImplementation::isPlaying() const {
    return _sound->isPlaying() && !_sound->isPaused();
}

void OPCricketStreamedPlayerImplementation::setSpeed(double speed) {
    _sound->setSpeed((float)speed);
}

double OPCricketStreamedPlayerImplementation::getSpeed() const {
    return _sound->getSpeed();
}

void OPCricketStreamedPlayerImplementation::setLooping(bool shouldLoop) {
    _sound->setLoopCount(shouldLoop ? -1 : 0);
}

bool OPCricketStreamedPlayerImplementation::isLooping() const {
    return _sound->getLoopCount() == -1;
}

void OPCricketStreamedPlayerImplementation::setVolume(float volume) {
    _sound->setVolume(volume);
}

float OPCricketStreamedPlayerImplementation::getVolume() const {
    return _sound->getVolume();
}

void OPCricketStreamedPlayerImplementation::setCurrentPositionInSamples(int sample) {
    _sound->setPlayPosition(sample);
    _lastPausePosition = sample;
}

int OPCricketStreamedPlayerImplementation::getCurrentPositionInSamples() const {
    if (_paused){
        return _lastPausePosition;
    }
    return _sound->getPlayPosition();
}

double OPCricketStreamedPlayerImplementation::getSourceSampleRate() {
    return _sound->getSampleRate();
}

OPCricketStreamedPlayerImplementation::~OPCricketStreamedPlayerImplementation() {
    OP_LOG("BEGIN");
    if (_sound){
        _sound->destroy();
        _sound = nullptr;
    }
    OP_LOG("END");
}
