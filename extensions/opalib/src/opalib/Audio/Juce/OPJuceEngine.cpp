//
//  OPJuceEngine.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 22/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPJuceEngine.hpp"
#include <cocos2d.h>
#include "opalib/Crash/OPCrashAnalytics.hpp"
#include "opalib/Debug/OPDebug.hpp"
#include "opalib/Datetime/OPTimer.hpp"
#include "opalib/Permissions/OPPermissions.hpp"
#include "opalib/Audio/OPAudioManager.hpp"
#include <opalib/Audio/Juce/OPJuceSynthesizerImplementation.hpp>
#include <opalib/Audio/Juce/OPJuceStreamedPlayerImplementation.hpp>
#include <cassert>
#include <opalib/Audio/Juce/OPJuceMemoryPlayerImplementation.hpp>

using namespace juce;


void OPJuceEngine::initialize() {
    OP_LOG("BEGIN");
    MessageManager::getInstance();
    reinitialize(0, 2);
    OP_LOG("END");
}

void OPJuceEngine::setInputChannels(int inputChannels) {
    OP_LOG("BEGIN");
    if (getAudioDeviceSetup().inputChannels != inputChannels) {
        OP_LOG("Setting input levels: " + std::to_string(inputChannels));
        reinitialize(inputChannels, 2);
    }
    OP_LOG("END");
}

void OPJuceEngine::reinitialize(int inputChannels, int outputChannels) {
    OP_LOG("BEGIN");
    closeAudioDevice();
    initialise(inputChannels, outputChannels, nullptr, true, {}, nullptr);

    if (getAudioDeviceSetup().inputChannels == 0) {
        OPAudioManager::shared()->updateSilentPlaying();
    }
    OP_LOG("END");
}

void OPJuceEngine::pause(){
    OP_LOG("BEGIN");
    closeAudioDevice();
    OP_LOG("END");
}

void OPJuceEngine::resume(){
    OP_LOG("BEGIN");
    restartLastAudioDevice();
    OP_LOG("END");
}

void OPJuceEngine::shutDown() {
    OP_LOG("BEGIN");
    closeAudioDevice();
//    MessageManager::deleteInstance();
    OP_LOG("END");
}

double OPJuceEngine::getDeviceSampleRate() {
    return getAudioDeviceSetup().sampleRate;
}

float OPJuceEngine::getInputLevel() {
    if (!_inputLevelMeter)
        return 0;

    return _inputLevelMeter->getCurrentLevel();
}

float OPJuceEngine::getOutputLevel() {
    if (!_outputLevelMeter)
        return 0;

    return _outputLevelMeter->getCurrentLevel();
}

void OPJuceEngine::setUseInputLevelMeter(bool shouldUse) {
    if (shouldUse) {
        _inputLevelMeter = getInputLevelGetter();
    }
    else {
        _inputLevelMeter.reset();
    }
}

void OPJuceEngine::setUseOutputLevelMeter(bool shouldUse) {
    if (shouldUse) {
        _outputLevelMeter = getOutputLevelGetter();
    }
    else {
        _outputLevelMeter.reset();
    }
}

int OPJuceEngine::getAudioBufferSize() {
    return getAudioDeviceSetup().bufferSize;
}

void OPJuceEngine::audioDeviceIOCallbackInt (const float** inputChannelData, int totalNumInputChannels, float** outputChannelData, int totalNumOutputChannels, int numSamples) {
    AudioDeviceManager::audioDeviceIOCallbackInt(inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    std::lock_guard<std::mutex> lock(_audioIOMutex);
    if (_audioIOCallback){
        _audioIOCallback(inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    }
}

int OPJuceEngine::activeInputChannels() {
    return getCurrentAudioDevice()->getActiveInputChannels().toInteger();
}

int OPJuceEngine::activeOutputChannels() {
    return getCurrentAudioDevice()->getActiveOutputChannels().toInteger();
}

OPSynthesizerImplementation* OPJuceEngine::createSynthesizerImplementation() {
    return new OPJuceSynthesizerImplementation(this);
}

OPPlayerImplementation* OPJuceEngine::createStreamedPlayerImplementation(const std::string& filepath) {
    return new OPJuceStreamedPlayerImplementation(this, filepath);
}

OPPlayerImplementation *OPJuceEngine::createMemoryPlayerImplementation(const std::string &filepath,
                                                                       const LoadAsyncHandler &loadedCallback) {
    return new OPJuceMemoryPlayerImplementation(this, filepath, loadedCallback);
}

void OPJuceEngine::setAudioIOCallback(OPAudioEngineImplementation::AudioIOCallback callback) {
    std::lock_guard<std::mutex> lock(_audioIOMutex);
    _audioIOCallback = std::move(callback);
}
