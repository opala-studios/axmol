//
//  OPAudioFormatReaderBufferSource.hpp
//  opalib-ios
//
//  Created by Ricardo on 17/01/22.
//  Copyright © 2022 Opala Studios. All rights reserved.
//

#ifndef OPAudioFormatReaderBufferSource_hpp
#define OPAudioFormatReaderBufferSource_hpp

#include <JuceHeader.h>
#include <opalib/Audio/OPAudioBuffer.hpp>

class OPStreamedAudioBuffer : public juce::TimeSliceClient {
public:
    
    OPStreamedAudioBuffer(juce::AudioFormatReader* sourceReader, int samplesPerChunk, int chunkCount);
    
    void setPosition(int position);
    
    int useTimeSlice() override;
    
    void read(juce::AudioBuffer<float>* dst, int dstStartSample, int srcStartSamples, int sampleCount);
    
private:
    
    int getPositionOnBuffer(int readerSamplePosition);
    int getPositionOnReader(int bufferSamplePosition);
    
private:
    juce::AudioFormatReader* _reader;
    std::unique_ptr<juce::AudioBuffer<float>> _buffer;
    int _bufferLoadPosition;
    int _requestedChunkStart;
    int _samplesReady;
    int _samplesPerChunk;
    int _chunkCount;
    bool _looping;
};



class OPStreamedAudioSource : public juce::PositionableAudioSource {
public:
    OPStreamedAudioSource (juce::AudioFormatReader* sourceReader,
                             bool deleteReaderWhenThisIsDeleted);

    /** Destructor. */
    ~OPStreamedAudioSource() override;
    
    void setLooping (bool shouldLoop) override;

    /** Returns whether loop-mode is turned on or not. */
    bool isLooping() const override                             { return _looping; }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Implementation of the AudioSource method. */
    void releaseResources() override;

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo&) override;

    //==============================================================================
    /** Implements the PositionableAudioSource method. */
    void setNextReadPosition (juce::int64 newPosition) override;

    /** Implements the PositionableAudioSource method. */
    juce::int64 getNextReadPosition() const override;

    /** Implements the PositionableAudioSource method. */
    juce::int64 getTotalLength() const override;
private:
    juce::OptionalScopedPointer<juce::AudioFormatReader> _reader;
    std::shared_ptr<OPStreamedAudioBuffer> _buffer;
    juce::int64 _nextPlayPos;
    bool _looping;
    
    juce::TimeSliceThread _backgroundThread;
    uint64_t _padding = 0;//TODO: check if backgroundThread will be changing this data content

};


#endif /* OPAudioFormatReaderBufferSource_hpp */
