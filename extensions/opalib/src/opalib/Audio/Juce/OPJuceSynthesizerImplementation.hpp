//
// Created by Ricardo on 13/10/21.
//

#ifndef OPJUCESYNTHESIZERIMPLEMENTATION_HPP
#define OPJUCESYNTHESIZERIMPLEMENTATION_HPP

#include <opalib/Audio/OPSynthesizerImplementation.hpp>
#include <opalib/Audio/Juce/OPJuceEngine.hpp>
#include <unordered_map>


class OPJuceSynthesizerImplementation : public OPSynthesizerImplementation, juce::AudioIODeviceCallback {
public:
    OPJuceSynthesizerImplementation(OPJuceEngine* engine);
    ~OPJuceSynthesizerImplementation();

    void play(OPSynthItem *item) override;
    void stop(OPSynthItem *item) override;
    void loadItem(OPSynthItem *item, const LoadItemHandler &handler) override;
    void unloadItem(OPSynthItem *item) override;
    void unloadAll() override;

    bool isLoaded(OPSynthItem* item) const override;
    
    void setIsLooping(OPSynthItem* item, bool looping) override;
    bool getIsLooping(OPSynthItem* item) const override;
    
    void setSpeed(float speed) override;
    void setSpeed(OPSynthItem* item, float speed) override;
    float getSpeed(OPSynthItem* item) const override;
    
    void setVolume(float volume) override;
    void setVolume(OPSynthItem* item, float volume) override;
    float getVolume(OPSynthItem* item) const override;
protected:
    void audioDeviceIOCallback (const float** inputChannelData,
                                        int numInputChannels,
                                        float** outputChannelData,
                                        int numOutputChannels,
                                        int numSamples) override;

    void audioDeviceAboutToStart (juce::AudioIODevice* device) override;

    void audioDeviceStopped() override;
    
private:
    OPJuceEngine* _engine = nullptr;
    std::unique_ptr<juce::Synthesiser> _synth;
    std::unique_ptr<juce::MidiMessageCollector> _midiCollector;
    std::unordered_map<OPSynthItem*, OPSamplerSound*> _itemToSampler;
};


#endif //OPJUCESYNTHESIZERIMPLEMENTATION_HPP
