//
// Created by Ricardo on 14/10/21.
//

#ifndef OPJUCEPLAYERIMPLEMENTATION_HPP
#define OPJUCEPLAYERIMPLEMENTATION_HPP

#include <opalib/Audio/OPPlayerImplementation.hpp>
#include <JuceHeader.h>

class OPJuceEngine;

class OPJucePlayerImplementation : public OPPlayerImplementation {
public:
    OPJucePlayerImplementation(OPJuceEngine* engine);
    ~OPJucePlayerImplementation() override;

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
protected:
    virtual bool isPlayerReady() const = 0;
    virtual std::unique_ptr<juce::PositionableAudioSource> createSource() = 0;

    void setup();
    void hook();
    void unhook();

private:
    OPJuceEngine* _engine;
    double _speed = 1.0;
    std::unique_ptr<juce::AudioTransportSource> _transportSource { nullptr };
    std::unique_ptr<juce::ResamplingAudioSource> _resamplerSource { nullptr };
    std::unique_ptr<juce::AudioSourcePlayer> _player { nullptr };
    std::unique_ptr<juce::PositionableAudioSource> _source { nullptr };
};


#endif //OPJUCEPLAYERIMPLEMENTATION_HPP
