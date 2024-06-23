//
//  OPJuceEngine.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 22/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPJuceEngine_hpp
#define OPJuceEngine_hpp

#include <JuceHeader.h>

#include <opalib/Audio/OPAudioEngine.hpp>

class OPJuceEngine : public OPAudioEngineImplementation, public juce::AudioDeviceManager{
public:

    void setUseInputLevelMeter(bool);
    void setUseOutputLevelMeter(bool) override;

    void setInputChannels(int inputChannels) override;

    float getInputLevel();
    float getOutputLevel() override;

    int activeInputChannels() override;

    int activeOutputChannels() override;

    double getDeviceSampleRate() override;

    int getAudioBufferSize() override;


protected:

    void audioDeviceIOCallbackInt(const float** inputChannelData,
                                  int totalNumInputChannels,
                                  float** outputChannelData,
                                  int totalNumOutputChannels,
                                  int numSamples) override;

    void pause() override;
    void resume() override;
    void shutDown() override;
    void initialize() override;

    void setAudioIOCallback(AudioIOCallback callback) override;

protected:

    OPSynthesizerImplementation *createSynthesizerImplementation() override;

    OPPlayerImplementation *createStreamedPlayerImplementation(const std::string &filepath) override;

    OPPlayerImplementation *createMemoryPlayerImplementation(const std::string &filepath,
                                                             const LoadAsyncHandler &loadedCallback) override;

    void reinitialize(int inputChannels, int outputChannels);

protected:

    AudioDeviceManager::LevelMeter::Ptr _inputLevelMeter;
    AudioDeviceManager::LevelMeter::Ptr _outputLevelMeter;
    AudioIOCallback _audioIOCallback;
    std::mutex _audioIOMutex;

};
#endif /* OPJuceEngine_hpp */
