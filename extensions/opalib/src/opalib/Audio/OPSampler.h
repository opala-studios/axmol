//
// Created by Ricardo on 7/21/2021.
//

#ifndef OPALIB_OPSAMPLER_H
#define OPALIB_OPSAMPLER_H

#include <JuceHeader.h>

class OPSamplerSound : public juce::SamplerSound {
public:
    using juce::SamplerSound::SamplerSound;

    void setVolume(float volume, bool isDecibel = false) {
        _volume = volume;
        _isDecibel = isDecibel;
        
        //if (_volume != 1.0f)
            //_useCustomVolume = true;
    }
    
    float getVolume() const { return _volume; }
    bool isDecibel() const { return _isDecibel; }
    bool useCustomVolume() const { return _useCustomVolume; }

protected:
    friend class OPSamplerVoice;
    
    float _volume = 1.0f;
    bool _isDecibel = false;
    bool _useCustomVolume = false;
};

class OPSamplerVoice : public juce::SamplerVoice {
public:
    void renderNextBlock(juce::AudioBuffer<float> &buffer, int startSample, int numSamples) override;
    bool canPlaySound (juce::SynthesiserSound* sound) override;
};


#endif //OPALIB_OPSAMPLER_H
