//
// Created by Ricardo on 7/21/2021.
//

#include "OPSampler.h"

void OPSamplerVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    if (auto* playingSound = static_cast<OPSamplerSound*> (getCurrentlyPlayingSound().get())) {

        auto& data = *playingSound->getAudioData();
        const float* const inL = data.getReadPointer (0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;
        
        auto volume = playingSound->getVolume();

        while (--numSamples >= 0)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                                       : l;

            auto envelopeValue = adsr.getNextSample() * volume;

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }

            sourceSamplePosition += pitchRatio * playingSound->getSpeed();
            

            if (sourceSamplePosition > playingSound->length)
            {
                if (playingSound->getIsLooping() && adsr.isActive()) {
                    sourceSamplePosition = 0;
                } else {
                    stopNote (0.0f, false);
                }
                break;
            }
        }
    }
}

bool OPSamplerVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<const OPSamplerSound*> (sound) != nullptr;
}
