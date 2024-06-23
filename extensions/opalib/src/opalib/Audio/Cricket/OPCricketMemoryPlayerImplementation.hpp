//
// Created by Ricardo on 18/10/21.
//

#ifndef OPCRICKETMEMORYPLAYERIMPLEMENTATION_HPP
#define OPCRICKETMEMORYPLAYERIMPLEMENTATION_HPP

#include <opalib/Audio/OPPlayerImplementation.hpp>
#include <opalib/Audio/OPAudioBuffer.hpp>
#include "OPCricketBufferSound.hpp"

class OPCricketMemoryPlayerImplementation : public OPPlayerImplementation, public OPAudioBufferHolder {
public:
    typedef std::function<void(bool)> LoadCallback;
public:
    OPCricketMemoryPlayerImplementation(const std::string& filepath, const LoadCallback& loadCallback);
    ~OPCricketMemoryPlayerImplementation();

    std::shared_ptr<OPAudioBuffer> getAudioBuffer() override;

    void play() override;

    void stop() override;
    
    void pause() override;

    void resume() override;
    
    bool isReady() const override;

    int getLengthInSamples() const override;

    double getLengthInSeconds() const override;

    bool isPlaying() const override;

    void setSpeed(double speed) override;

    double getSpeed() const override;

    void setLooping(bool shouldLoop) override;

    bool isLooping() const override;

    void setVolume(float d) override;

    float getVolume() const override;

    void setCurrentPositionInSamples(int i) override;

    int getCurrentPositionInSamples() const override;

    double getSourceSampleRate() override;

private:
    std::shared_ptr<OPAudioBuffer> _buffer;
    OPCricketBufferSound* _sound = nullptr;
    int _lastPausePosition = 0;
    bool _paused = false;

};


#endif //OPCRICKETMEMORYPLAYERIMPLEMENTATION_HPP
