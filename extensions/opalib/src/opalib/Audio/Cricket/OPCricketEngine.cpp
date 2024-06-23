//
// Created by Opala Teste on 28/09/21.
//

#include "OPCricketEngine.hpp"
#include "opalib/Audio/OPAudioBuffer.hpp"
#include "opalib/Audio/Cricket/OPCricketSynthesizerImplementation.hpp"
#include "opalib/Audio/Cricket/OPCricketMemoryPlayerImplementation.hpp"
#include "opalib/Audio/Cricket/OPCricketStreamedPlayerImplementation.hpp"
#include "opalib/Audio/Cricket/OPCricketAudioStream.hpp"
#include <ck/ck.h>
#include <ck/config.h>
#include <cocos2d.h>
#include <ck/audio/audiograph.h>
#include <ck/core/system.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <ck/audio/audio_android.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <ck/audio/audio_ios.h>
#endif


static CkCustomStream* createCustomStream(const char* path, void* data){
    return new OPCricketAudioStream(path);
}

void OPCricketEngine::update(float dt) {
    CkUpdate();
}

void OPCricketEngine::pause() {
    CkSuspend();
}

void OPCricketEngine::resume() {
    CkResume();
}

void OPCricketEngine::shutDown() {
    cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    CkShutdown();
}

void OPCricketEngine::initialize() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CkConfig config(cocos2d::JniHelper::getJavaVM(), cocos2d::JniHelper::getActivity());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CkConfig config;
#endif

    CkInit(&config);

    CkSound::setCustomStreamHandler(createCustomStream, nullptr);

    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);


    Cki::AudioGraph::get()->setRenderCallback([this](void* data, int sampleCount, int sampleSize, int channelCount){
        std::lock_guard<std::mutex> lock(_audioIOMutex);
        //TODO: find another way to get the current buffer size, as this size may not be the size of the total rendered buffer
        _bufferSize = sampleCount;

        if (!_audioIOCallback){
            return;
        }

        int remainingSamples = sampleCount;

        const int maxSamplesPerChannel = 512;
        float outputChannel1[maxSamplesPerChannel];
        float outputChannel2[maxSamplesPerChannel];
        float* outputChannels[2]{outputChannel1, outputChannel2};

        while (remainingSamples > 0){

            int currentSampleCount = std::min(remainingSamples, maxSamplesPerChannel);

            juce::AudioDataConverters::deinterleaveSamples((float*)data, outputChannels, currentSampleCount, channelCount);
            _audioIOCallback(nullptr, 0, outputChannels, channelCount, currentSampleCount);

            remainingSamples -= currentSampleCount;
        }
    });
}

void OPCricketEngine::setInputChannels(int channels) {
    assert(channels == 0 && "Input channels are not available in cricket");
}

int OPCricketEngine::activeInputChannels() {
    return 0;
}

int OPCricketEngine::activeOutputChannels() {
    return 2;
}

float OPCricketEngine::getOutputLevel() {
    return 0;//TODO: modify cricket so we can get some form of output level
}

void OPCricketEngine::setUseOutputLevelMeter(bool) {
    //TODO: implement output level meter
}

double OPCricketEngine::getDeviceSampleRate() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return Cki::Audio::getNativeSampleRate();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return Cki::Audio::getSessionSampleRate();
#endif
}

int OPCricketEngine::getAudioBufferSize() {
    return _bufferSize;
}

OPSynthesizerImplementation *OPCricketEngine::createSynthesizerImplementation() {
    return new OPCricketSynthesizerImplementation(16);
}

OPPlayerImplementation *
OPCricketEngine::createStreamedPlayerImplementation(const std::string &filepath) {
    return new OPCricketStreamedPlayerImplementation(filepath);
}

OPPlayerImplementation *
OPCricketEngine::createMemoryPlayerImplementation(const std::string &filePath,
                                                  const LoadAsyncHandler &loadedCallback) {
    return new OPCricketMemoryPlayerImplementation(filePath, loadedCallback);
}

void OPCricketEngine::setAudioIOCallback(OPAudioEngineImplementation::AudioIOCallback callback) {
    std::lock_guard<std::mutex> lock(_audioIOMutex);
    _audioIOCallback = std::move(callback);
}
