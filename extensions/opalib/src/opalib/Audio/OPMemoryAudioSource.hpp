//
// Created by Talles Borges  on 17/09/20.
//

#ifndef OPMEMORYAUDIOSOURCE_HPP_
#define OPMEMORYAUDIOSOURCE_HPP_

#include <JuceHeader.h>

class OPMemoryAudioSource : public juce::PositionableAudioSource
{
public:
    OPMemoryAudioSource(juce::AudioBuffer<float>& buffer, bool isCurrentlyLooping = false);

    OPMemoryAudioSource(juce::AudioBuffer<float>&& buffer, bool isCurrentlyLooping = false);

    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Implementation of the AudioSource method. */
    void releaseResources() override;

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    //==============================================================================
    /** Implementation of the PositionableAudioSource method. */
    void setNextReadPosition (juce::int64 newPosition) override;

    /** Implementation of the PositionableAudioSource method. */
    juce::int64 getNextReadPosition() const override;

    /** Implementation of the PositionableAudioSource method. */
    juce::int64 getTotalLength() const override;

    //==============================================================================
    /** Implementation of the PositionableAudioSource method. */
    bool isLooping() const override;

    /** Implementation of the PositionableAudioSource method. */
    void setLooping (bool shouldLoop) override;
protected:
    //==============================================================================
    juce::AudioBuffer<float> buffer;
    juce::int64 position = 0;
    bool isCurrentlyLooping;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OPMemoryAudioSource)

};

#endif // OPMEMORYAUDIOSOURCE_HPP_
