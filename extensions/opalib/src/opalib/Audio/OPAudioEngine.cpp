//
// Created by Ricardo on 07/10/21.
//

#include <opalib/Debug/OPDebug.hpp>
#include <opalib/Audio/Cricket/OPCricketEngine.hpp>
#include <opalib/Audio/Juce/OPJuceEngine.hpp>
#include "OPAudioEngine.hpp"
#include "OPAudioManager.hpp"
#include <opalib/Json/OPJson.hpp>

void OPAudioEngine::AudioIOCallbackArray::addAudioIOCallback(OPBaseAudioIOCallback* callback) {
    _ioCallbacks.emplace_back(callback);
    sort();;
}

void OPAudioEngine::AudioIOCallbackArray::removeAudioIOCallback(OPBaseAudioIOCallback* callback) {
    auto it = std::find_if(_ioCallbacks.begin(), _ioCallbacks.end(), [callback](OPBaseAudioIOCallback* element){
        return element == callback;
    });
    assert(it != _ioCallbacks.end() && "Tried to double remove an invalid audio io callback id.");
    _ioCallbacks.erase(it);
    sort();
}

void OPAudioEngine::AudioIOCallbackArray::sort() {
    std::sort(_ioCallbacks.begin(), _ioCallbacks.end(),
    [](OPBaseAudioIOCallback* lhs, OPBaseAudioIOCallback* rhs){
        return lhs->getPriority() < rhs->getPriority();
    });
}

OPAudioEngine::OPAudioEngine() {
    // Guarantees that the message manager will be available to all
    // (we may use it even when using cricket as our audio engine)

}

OPAudioEngine::~OPAudioEngine() {
    shutDown();
}

void OPAudioEngine::loadConfiguration(const std::string& filePath){
    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    auto document = OPJson::document();
    if (OPJson::parseFile(fullPath, document)){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        _config.engineType = document["config"]["engine"]["android"].GetString();
#else
        _config.engineType = document["config"]["engine"]["ios"].GetString();
#endif
    }
    
}

void OPAudioEngine::changeImplementation(OPAudioEngineImplementation *implementation, bool autoInitialize) {
    if (_implementation){
        shutDown();
    }
    _implementation.reset(implementation);
    _implementation->setAudioIOCallback(
            [this](
                    const float** inputChannelData,
                    int totalNumInputChannels,
                    float** outputChannelData,
                    int totalNumOutputChannels,
                    int numSamples){

                audioDeviceIOCallback(
                        inputChannelData,
                        totalNumInputChannels,
                        outputChannelData,
                        totalNumOutputChannels,
                        numSamples
                );
            });
    if (autoInitialize){
        initialize();
    }
}

void OPAudioEngine::setImplementationReadyCallback(ImplementationReadyCallback callback){
    _implementationReadyCallback = std::move(callback);
}

void OPAudioEngine::incrementPendingImplementationCounter(){
    _pendingImplementationCounter++;
}

void OPAudioEngine::decrementPendingImplementationCounter(){
    _pendingImplementationCounter--;
    if (isImplementationReady() && _implementationReadyCallback){
        _implementationReadyCallback();
    }
}

bool OPAudioEngine::isImplementationReady() const {
    return _pendingImplementationCounter == 0;
}

void OPAudioEngine::initialize() {
    OP_LOG("BEGIN");
    loadConfiguration("config/audio.json");
    _eventBus = std::make_shared<Dexode::EventBus>();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    juce::MessageManager::getInstance();
#endif
    if (_config.engineType == "juce"){
        changeImplementation<OPJuceEngine>(false);
    }
    else {
        changeImplementation<OPCricketEngine>(false);
    }
    juce::MessageManager::getInstance();
    _implementation->initialize();
    _eventBus->notify(EventCreateImplementation{});
    OP_LOG("END");
}

void OPAudioEngine::shutDown() {
    OP_LOG("BEGIN");
    _eventBus->notify(EventDestroyImplementation{});
    _implementation->shutDown();
    OP_LOG("END");
}

void OPAudioEngine::pause() {
    OP_LOG("BEGIN");
    if (_paused) {
        OP_LOG("Audio engine is already paused - END");
        return;
    }

    _paused = true;
    _eventBus->notify(EventAudioPause());
    _implementation->pause();
    OPAudioManager::shared()->updateSilentPlaying();
    OP_LOG("END");
}

void OPAudioEngine::resume() {
    OP_LOG("BEGIN");
    if (!_paused) {
        OP_LOG("Audio engine is already running - END");
        return;
    }

    _paused = false;
    _implementation->resume();
    _eventBus->notify(EventAudioResume());
    OPAudioManager::shared()->updateSilentPlaying();
    OP_LOG("END");
}

void OPAudioEngine::setInputChannels(int channels) {
    _implementation->setInputChannels(channels);
}

int OPAudioEngine::activeInputChannels() {
    return _implementation->activeInputChannels();
}

int OPAudioEngine::activeOutputChannels() {
    return _implementation->activeOutputChannels();
}

double OPAudioEngine::getDeviceSampleRate() {
    return _implementation->getDeviceSampleRate();
}

int OPAudioEngine::getAudioBufferSize() {
    return _implementation->getAudioBufferSize();
}

float OPAudioEngine::getOutputLevel(){
    return _implementation->getOutputLevel();
}

void OPAudioEngine::setUseOutputLevelMeter(bool shouldUse){
    _implementation->setUseOutputLevelMeter(shouldUse);
}

OPSynthesizerImplementation *OPAudioEngine::createSynthesizerImplementation() {
    return _implementation->createSynthesizerImplementation();
}

OPPlayerImplementation *
OPAudioEngine::createStreamedPlayerImplementation(const std::string &filepath) {
    return _implementation->createStreamedPlayerImplementation(filepath);
}

OPPlayerImplementation *OPAudioEngine::createMemoryPlayerImplementation(const std::string &filePath,
                                                                        const LoadAsyncHandler &loadedCallback) {
    return _implementation->createMemoryPlayerImplementation(filePath, loadedCallback);
}

void OPAudioEngine::addAudioIOCallback(OPBaseAudioIOCallback* callback) {
    std::lock_guard<std::mutex> lock{_ioCallbackMutex};
    _ioCallbackArray.addAudioIOCallback(callback);
}

void OPAudioEngine::removeAudioIOCallback(OPBaseAudioIOCallback* callback) {
    std::lock_guard<std::mutex> lock{_ioCallbackMutex};
    _ioCallbackArray.removeAudioIOCallback(callback);
}

void OPAudioEngine::audioDeviceIOCallback(const float **inputChannelData, int totalNumInputChannels,
                                          float **outputChannelData, int totalNumOutputChannels,
                                          int numSamples) {
    std::lock_guard<std::mutex> lock{_ioCallbackMutex};

    for (auto& callback : _ioCallbackArray){
        callback->audioDeviceIOCallbackInt(inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    }
}
