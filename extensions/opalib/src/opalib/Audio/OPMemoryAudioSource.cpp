//
// Created by Talles Borges  on 17/09/20.
//

#include "OPMemoryAudioSource.hpp"

//==============================================================================

OPMemoryAudioSource::OPMemoryAudioSource(juce::AudioBuffer<float> &buffer,
                                         bool isCurrentlyLooping) :
                                         buffer(buffer),
                                         isCurrentlyLooping(isCurrentlyLooping){

}

OPMemoryAudioSource::OPMemoryAudioSource(juce::AudioBuffer<float>&& buffer,
                                         bool isCurrentlyLooping) :
                                         buffer(std::move(buffer)),
                                         isCurrentlyLooping(isCurrentlyLooping){

}



void OPMemoryAudioSource::prepareToPlay (int /*samplesPerBlockExpected*/, double /*sampleRate*/)
{
    position = 0;
}

void OPMemoryAudioSource::releaseResources()   {}

void OPMemoryAudioSource::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto& dst = *bufferToFill.buffer;
    auto channels = juce::jmax (dst.getNumChannels(), buffer.getNumChannels());
    auto max = 0, pos = 0;
    auto n = buffer.getNumSamples(), m = bufferToFill.numSamples;
    
    int i;
    for (i = position; (i < n || isCurrentlyLooping) && (pos < m); i += max)
    {
        max = juce::jmin(m - pos, n - (i % n));
        
        int ch = 0;
        for (; ch < channels; ++ch)
            dst.copyFrom(ch, bufferToFill.startSample + pos, buffer, buffer.getNumChannels() > 1 ? ch : 0, i % n, max);
        
        for (; ch < dst.getNumChannels(); ++ch)
            dst.clear(ch, bufferToFill.startSample + pos, max);
        
        pos += max;
    }
    
    if (pos < m)
        dst.clear(bufferToFill.startSample + pos, m - pos);
    
    position = isCurrentlyLooping ? (i % n) : i;
}

//==============================================================================
void OPMemoryAudioSource::setNextReadPosition (juce::int64 newPosition)
{
    position = (int) newPosition;
}

juce::int64 OPMemoryAudioSource::getNextReadPosition() const
{
    return position;
}

juce::int64 OPMemoryAudioSource::getTotalLength() const
{
    return buffer.getNumSamples();
}

//==============================================================================
bool OPMemoryAudioSource::isLooping() const
{
    return isCurrentlyLooping;
}

void OPMemoryAudioSource::setLooping (bool shouldLoop)
{
    isCurrentlyLooping = shouldLoop;
}


