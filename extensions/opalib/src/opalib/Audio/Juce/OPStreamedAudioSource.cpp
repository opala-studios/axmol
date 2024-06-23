//
//  OPAudioFormatReaderBufferSource.cpp
//  opalib-ios
//
//  Created by Ricardo on 17/01/22.
//  Copyright © 2022 Opala Studios. All rights reserved.
//

#include "OPStreamedAudioSource.hpp"

static void read(juce::AudioBuffer<float>* src, int srcStartSample, juce::AudioBuffer<float>* dst, int dstStartSample, int sampleCount){
    int srcChannels = src->getNumChannels();
    int dstChannels = dst->getNumChannels();
    
    for (int ch = 0; ch < dstChannels; ch++){
        dst->copyFrom(ch, dstStartSample, *src, ch % srcChannels, srcStartSample, sampleCount);
    }
}

OPStreamedAudioBuffer::OPStreamedAudioBuffer(juce::AudioFormatReader* sourceReader, int samplesPerChunk, int chunkCount) :
    _reader(sourceReader),
    _bufferLoadPosition(0),
    _requestedChunkStart(0),
    _samplesReady(0),
    _samplesPerChunk(samplesPerChunk),
    _chunkCount(chunkCount),
    _looping(false)
{
    _buffer = std::make_unique<juce::AudioBuffer<float>>(_reader->numChannels, samplesPerChunk * chunkCount);
    
    for(int i = 0; i < chunkCount; i++){
        useTimeSlice();
    }
}

void OPStreamedAudioBuffer::setPosition(int position){
    int newChunkStart = (position / _samplesPerChunk) * _samplesPerChunk;
    
    int diff = newChunkStart - _requestedChunkStart;
    if (diff > 0){
        _samplesReady = std::max(_samplesReady - diff, 0);
    }
    else if (diff < 0){
        _samplesReady = 0;
    }
    _requestedChunkStart = newChunkStart;
    _bufferLoadPosition = getPositionOnBuffer(_requestedChunkStart + _samplesReady);
}

int OPStreamedAudioBuffer::getPositionOnBuffer(int readerSamplePosition){
    return readerSamplePosition % _buffer->getNumSamples();
}

int OPStreamedAudioBuffer::getPositionOnReader(int bufferSamplePosition){
    return 0;
}

int OPStreamedAudioBuffer::useTimeSlice() {
    if (_samplesReady >= _samplesPerChunk * _chunkCount){
        return 0;
    }
    
    int bufferStart = _bufferLoadPosition % _buffer->getNumSamples();
    int bufferEnd = (_bufferLoadPosition + _samplesPerChunk) % _buffer->getNumSamples();
    int readerStartPosition = _requestedChunkStart + _samplesReady;
    
    //read will be split into 2 parts, one at the end of the buffer
    //and another at the start
    if (bufferStart > bufferEnd){

        int firstReadSampleCount = _buffer->getNumSamples() - bufferStart;
        _reader->read(_buffer.get(), bufferStart, firstReadSampleCount, readerStartPosition, true, true);

        //the second read will read the remaining samples, which is simply bufferEnd
        int secondReadSampleCount = bufferEnd;
        _reader->read(_buffer.get(), 0, secondReadSampleCount, readerStartPosition + firstReadSampleCount, true, true);
    }
    else {
        int samplesToRead = bufferEnd - bufferStart;
        _reader->read(_buffer.get(), bufferStart, samplesToRead, readerStartPosition, true, true);
    }
  
    
    _bufferLoadPosition = bufferEnd;
    
    _samplesReady += _samplesPerChunk;
    
    return 0;
}

void OPStreamedAudioBuffer::read(juce::AudioBuffer<float>* dst, int dstStartSample, int srcStartSamples, int sampleCount){
    
    //If this assert fails, it means that you don't have enough samples loaded
    //Maybe try changing chunk size and/or count?
//    if (srcStartSamples < _requestedChunkStart || srcStartSamples + sampleCount >= _requestedChunkStart + _samplesReady){
//        jassertfalse;
//    }
    
    int readStartOnBuffer = getPositionOnBuffer(srcStartSamples);
    int readEndOnBuffer = getPositionOnBuffer(srcStartSamples + sampleCount);
    
    if (readStartOnBuffer > readEndOnBuffer){
        int firstReadSampleCount = _buffer->getNumSamples() - readStartOnBuffer;
        ::read(_buffer.get(), readStartOnBuffer, dst, dstStartSample, firstReadSampleCount);
        
        int secondReadSampleCount = readEndOnBuffer;
        ::read(_buffer.get(), 0, dst, dstStartSample + firstReadSampleCount, secondReadSampleCount);
    }
    else {
        int samplesToRead = readEndOnBuffer - readStartOnBuffer;
        ::read(_buffer.get(), readStartOnBuffer, dst, dstStartSample, samplesToRead);
    }
    
//    for (int sample = 0; sample < sampleCount; sample++) {
//        std::cout << dst->getSample(0, sample) << std::endl;
//    }

    
//    for (int channel = 0; channel < totalNumOutputChannels; channel++) {
//        for (int sample = 0; sample < numSamples; sample++) {
//            std::cout << outputChannelData[channel][sample] << std::endl;
//        }
//    }
}


OPStreamedAudioSource::OPStreamedAudioSource (juce::AudioFormatReader* sourceReader,
                                                                  bool deleteReaderWhenThisIsDeleted) :
    _reader(sourceReader, deleteReaderWhenThisIsDeleted),
    _nextPlayPos(0),
    _backgroundThread("streamed audio buffer"),
    _looping(false){
    jassert(_reader != nullptr);
    _buffer = std::make_shared<OPStreamedAudioBuffer>(sourceReader, 1024, 3);
    _backgroundThread.addTimeSliceClient(_buffer.get());
}

/** Destructor. */
OPStreamedAudioSource::~OPStreamedAudioSource() {
    
}

void OPStreamedAudioSource::setLooping(bool shouldLoop) {
    _looping = shouldLoop;
}

void OPStreamedAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    _backgroundThread.startThread();
}

/** Implementation of the AudioSource method. */
void OPStreamedAudioSource::releaseResources() {
    _backgroundThread.stopThread(100);
}

/** Implementation of the AudioSource method. */
void OPStreamedAudioSource::getNextAudioBlock (const juce::AudioSourceChannelInfo& info) {
    if (info.numSamples > 0)
    {
        const juce::int64 start = _nextPlayPos;

        if (_looping)
        {
            const juce::int64 newStart = start % _reader->lengthInSamples;
            const juce::int64 newEnd = (start + info.numSamples) % _reader->lengthInSamples;

            if (newEnd > newStart)
            {
                _buffer->read(info.buffer, info.startSample, newStart, (int)(newEnd - newStart));
            }
            else
            {
                const int endSamps = (int) (_reader->lengthInSamples - newStart);

                _buffer->read(info.buffer, info.startSample, newStart, endSamps);
                
                _buffer->read(info.buffer, info.startSample + endSamps, 0, newEnd);
            }

            _nextPlayPos = newEnd;
        }
        else
        {
            _buffer->read(info.buffer, info.startSample, start, info.numSamples);
            
            _nextPlayPos += info.numSamples;
        }
        _buffer->setPosition(_nextPlayPos);
    }
}

//==============================================================================
/** Implements the PositionableAudioSource method. */
void OPStreamedAudioSource::setNextReadPosition (juce::int64 newPosition) {
    _nextPlayPos = newPosition;
    _buffer->setPosition(newPosition);
}

/** Implements the PositionableAudioSource method. */
juce::int64 OPStreamedAudioSource::getNextReadPosition() const {
    return _looping ? _nextPlayPos % _reader->lengthInSamples
                   : _nextPlayPos;
}

/** Implements the PositionableAudioSource method. */
juce::int64 OPStreamedAudioSource::getTotalLength() const {
    return _reader->lengthInSamples;
}
