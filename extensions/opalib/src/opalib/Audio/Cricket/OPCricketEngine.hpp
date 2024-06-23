//
// Created by Ricardo on 28/09/21.
//

#ifndef OPCRICKETENGINE_HPP
#define OPCRICKETENGINE_HPP

#include <opalib/Audio/OPAudioEngine.hpp>


class OPCricketEngine : public OPAudioEngineImplementation {
public:
    void update(float dt);
protected:
    void pause() override;
    void resume() override;
    void shutDown() override;
    void initialize() override;

    void setAudioIOCallback(AudioIOCallback callback) override;

    OPSynthesizerImplementation *createSynthesizerImplementation() override;

    OPPlayerImplementation *
    createStreamedPlayerImplementation(const std::string &filepath) override;

    OPPlayerImplementation *createMemoryPlayerImplementation(const std::string &filePath,
                                                             const LoadAsyncHandler &loadedCallback) override;

public:

    void setInputChannels(int channels) override;

    int activeInputChannels() override;

    int activeOutputChannels() override;

    double getDeviceSampleRate() override;

    int getAudioBufferSize() override;
    
    float getOutputLevel() override;
    
    void setUseOutputLevelMeter(bool) override;

private:
    AudioIOCallback _audioIOCallback;
    std::mutex _audioIOMutex;
    int _bufferSize = 0;
};


#endif //OPCRICKETENGINE_HPP
