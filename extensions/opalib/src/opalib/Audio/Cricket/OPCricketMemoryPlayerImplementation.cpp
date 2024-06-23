//
// Created by Ricardo on 18/10/21.
//

#include "OPCricketMemoryPlayerImplementation.hpp"
#include <opalib/Debug/OPDebug.hpp>

OPCricketMemoryPlayerImplementation::OPCricketMemoryPlayerImplementation(
        const std::string &filepath,
        const OPCricketMemoryPlayerImplementation::LoadCallback &loadCallback) {
    OP_LOG("BEGIN");
    _buffer = OPAudioBuffer::create(filepath, OPChannelLayout::INTERLEAVED);
    _buffer->loadAsync([this, loadCallback](bool success){
        OP_LOG("Loaded - " + std::to_string(success));
        if (success){
            _sound = new OPCricketBufferSound(_buffer);
        }
        loadCallback(success);
    });
    OP_LOG("END");
}

OPCricketMemoryPlayerImplementation::~OPCricketMemoryPlayerImplementation() {
    OP_LOG("BEGIN");
    if (_sound) {
        _sound->destroy();
        _sound = nullptr;
    }
    OP_LOG("END");
}

std::shared_ptr<OPAudioBuffer> OPCricketMemoryPlayerImplementation::getAudioBuffer() {
    return _buffer;
}

bool OPCricketMemoryPlayerImplementation::isReady() const {
    return _sound != nullptr;
}

void OPCricketMemoryPlayerImplementation::play() {
    if (_sound){
        if (_paused){
            resume();
            return;
        }
        _sound->play();
    }
}

void OPCricketMemoryPlayerImplementation::stop() {
    if (_sound) {
        _sound->stop();
    }
}

void OPCricketMemoryPlayerImplementation::pause() {
    if (_sound){
        _lastPausePosition = getCurrentPositionInSamples();
        _sound->stop();
        _sound->setPlayPosition(_lastPausePosition);
        _paused = true;
    }
}

void OPCricketMemoryPlayerImplementation::resume() {
    if (_sound){
        if (!_paused){
            return;
        }
        _sound->play();
        _sound->setPlayPosition(_lastPausePosition);
        _paused = false;
    }
}

int OPCricketMemoryPlayerImplementation::getLengthInSamples() const {
    return _buffer->getSampleCount();
}

double OPCricketMemoryPlayerImplementation::getLengthInSeconds() const {
    return _buffer->getLenghtInSeconds();
}

bool OPCricketMemoryPlayerImplementation::isPlaying() const {
    if (!_sound){
        return false;
    }
    return _sound->isPlaying() && !_sound->isPaused();
}

void OPCricketMemoryPlayerImplementation::setSpeed(double speed) {
    if (_sound) {
        _sound->setSpeed((float) speed);
    }
}

double OPCricketMemoryPlayerImplementation::getSpeed() const {
    if (!_sound){
        return 0;
    }
    return _sound->getSpeed();
}

void OPCricketMemoryPlayerImplementation::setLooping(bool shouldLoop) {
    if (_sound) {
        _sound->setLoopCount(shouldLoop ? -1 : 0);
    }
}

bool OPCricketMemoryPlayerImplementation::isLooping() const {
    if (!_sound){
        return false;
    }
    return _sound->getLoopCount() == -1;
}

void OPCricketMemoryPlayerImplementation::setVolume(float volume) {
    if (_sound) {
        _sound->setVolume(volume);
    }
}

float OPCricketMemoryPlayerImplementation::getVolume() const {
    if (!_sound){
        return 0;
    }
    return _sound->getVolume();
}

void OPCricketMemoryPlayerImplementation::setCurrentPositionInSamples(int sample) {
    if (_sound){
        _sound->setPlayPosition(sample);
        _lastPausePosition = sample;
    }
}

int OPCricketMemoryPlayerImplementation::getCurrentPositionInSamples() const {
    if (!_sound){
        return 0;
    }
    
    if (_paused){
        return _lastPausePosition;
    }
    
    return _sound->getPlayPosition();
}

double OPCricketMemoryPlayerImplementation::getSourceSampleRate() {
    return _buffer->getSampleRate();
}
