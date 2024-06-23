//
// Created by Ricardo on 22/10/21.
//

#ifndef OPAUDIOENGINEIMPLEMENTATION_HPP
#define OPAUDIOENGINEIMPLEMENTATION_HPP

#include <opalib/Audio/OPAudioBuffer.hpp>

class OPSynthesizerImplementation;
class OPPlayerImplementation;

class OPAudioEngineImplementation {
public:
    typedef std::function<void(const float** inputChannelData,
                               int totalNumInputChannels,
                               float** outputChannelData,
                               int totalNumOutputChannels,
                               int numSamples)> AudioIOCallback;
public:
    virtual ~OPAudioEngineImplementation() = default;
    virtual int activeInputChannels() = 0;
    virtual int activeOutputChannels() = 0;
    virtual void setInputChannels(int channels) = 0;
    virtual double getDeviceSampleRate() = 0;
    virtual int getAudioBufferSize() = 0;
    virtual float getOutputLevel() = 0;
    virtual void setUseOutputLevelMeter(bool shouldUse) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void shutDown() = 0;
    virtual void initialize() = 0;
    virtual void setAudioIOCallback(AudioIOCallback callback) = 0;
    virtual OPSynthesizerImplementation* createSynthesizerImplementation() = 0;
    virtual OPPlayerImplementation* createStreamedPlayerImplementation(const std::string &filepath) = 0;
    virtual OPPlayerImplementation* createMemoryPlayerImplementation(const std::string& filePath, const LoadAsyncHandler &loadedCallback) = 0;

};

#endif //OPAUDIOENGINEIMPLEMENTATION_HPP
