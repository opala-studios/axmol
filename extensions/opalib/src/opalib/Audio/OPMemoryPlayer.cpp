//
//  OPMemoryPlayer.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 09/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include <opalib/Debug/OPDebug.hpp>
#include "OPMemoryPlayer.hpp"
#include <SoundTouch.h>
#include <opalib/Misc/OPCocosThread.hpp>
#include <utility>

template<typename T>
void merge_arrays(T* __restrict dst, const T* __restrict src1, const T* __restrict src2, size_t srcSize){
    size_t dstIndex = 0, srcIndex = 0;
    while(srcIndex < srcSize){
        dst[dstIndex] = src1[srcIndex];
        dst[dstIndex + 1] = src2[srcIndex];
        dstIndex += 2;
        srcIndex++;
    }
}

template<typename T>
void distribute_array(const T* __restrict src, T* __restrict dst1, T* __restrict dst2, size_t dstSize){
    size_t dstIndex = 0, srcIndex = 0;
    while (dstIndex < dstSize){
        dst1[dstIndex] = src[srcIndex];
        dst2[dstIndex] = src[srcIndex + 1];
        dstIndex++;
        srcIndex += 2;
    }
}


std::unique_ptr<OPMemoryPlayer> OPMemoryPlayer::createUnique(const std::string& filePath, const LoadAsyncHandler& loadedCallback)
{
    return std::unique_ptr<OPMemoryPlayer>(new OPMemoryPlayer(filePath, loadedCallback));
}

std::shared_ptr<OPMemoryPlayer> OPMemoryPlayer::createShared(const std::string& filePath,const LoadAsyncHandler& loadedCallback)
{
    return std::shared_ptr<OPMemoryPlayer>(new OPMemoryPlayer(filePath, loadedCallback));
}


OPMemoryPlayer::OPMemoryPlayer(const std::string& filePath, const LoadAsyncHandler& loadedCallback) :
    _filepath(filePath), _loadedCallback(loadedCallback)
{
    OP_LOG("BEGIN");
        createImplementation(filePath, loadedCallback);
    OP_LOG("END");
}

std::shared_ptr<OPAudioBuffer> OPMemoryPlayer::getAudioBuffer()
{
    return _implementation->getAudioBuffer();
}

void OPMemoryPlayer::playRange(OPWaveformRange* range)
{
    CC_SAFE_RELEASE_NULL(_currentRange);
    _currentRange = range;
    _currentRange->retain();

    setCurrentPositionInSamples(_currentRange->getStartInSamples());
    setCurrentEndPositionInSamples();

    _isPlayingRange = true;
    resumeTimer();
    play();
}

void OPMemoryPlayer::stop()
{
    OP_LOG("BEGIN");
    OPPlayer::stop();
    
    if (_isPlayingRange) {
        stopTimer();
        setCurrentPositionInSamples(_currentRange->getStartInSamples());
    }
    OP_LOG("END");
//    clearRangeIfNeeded();
}


void OPMemoryPlayer::setCurrentEndPositionInSamples()
{
    if (!getAudioBuffer()->isLoaded()){
        return;
    }
    
    int endPosInSamples = juce::jmin(getAudioBuffer()->getSampleCount(), _currentRange->getEndInSamples());
    endPosInSamples = juce::jmax(endPosInSamples,0);
    _forcedEndPosition = endPosInSamples;
}

void OPMemoryPlayer::update(float dt)
{
    if(!_isReady)
        return;
    
    OPPlayer::update(dt);
    
    if(_isPlayingRange)
    {
        if(_forcedEndPosition <= getCurrentPositionInSamples())
        {
            if(isLooping())
                setCurrentPositionInSamples(_currentRange->getStartInSamples());
            else{
                stop();
            }
        }
    }
}

double OPMemoryPlayer::getRangeDuration(OPWaveformRange *range)
{
    if (getSourceSampleRate() > 0.0)
        return (double) (range->getEndInSamples() - range->getStartInSamples()) / getSourceSampleRate();

    return 0.0;
}


void OPMemoryPlayer::clearRangeIfNeeded()
{
    if(_isPlayingRange)
    {
        _isPlayingRange = false;
        
        if(_forcedEndPosition != -1)
           _forcedEndPosition = -1;
        
        CC_SAFE_RELEASE_NULL(_currentRange);
    }

    stopTimer();
}

OPMemoryPlayer::~OPMemoryPlayer()
{
    OP_LOG("BEGIN");
    CC_SAFE_RELEASE_NULL(_currentRange);
    OPPlayer::stop();
    OP_LOG("END");
}

void OPMemoryPlayer::changeTempoAsync(
        float tempo,
        int bufferCount,
        const ChangeTempoBufferProcessedHandler &onBufferProcessed) {
    
    float tempoChange = ((tempo - _currentTempo) / _currentTempo) * 100;
    _currentTempo = tempo;
    _changeTempoTask.setTask(1,
            [this, tempoChange, bufferCount, onBufferProcessed]
            (size_t workerIndex, size_t workerCount) {

        auto buffer = getAudioBuffer();
        auto sampleBuffer = buffer->getSampleBuffer();
        double sampleRate = buffer->getSampleRate();
        size_t channelCount = std::min(buffer->getChannelCount(), 2);
        size_t inputSampleCount = buffer->getSampleCount();
        size_t samplesPerBuffer = inputSampleCount / bufferCount;
        size_t totalOutputSampleCount = 0;

        const float* inputChannels = new float[inputSampleCount * channelCount];
        buffer->getSampleBuffer()->copyToInterleaved(0, const_cast<float*>(inputChannels), inputSampleCount);
        
        float* outputChannels = new float[samplesPerBuffer * channelCount];
        memset(outputChannels, 0, samplesPerBuffer * sizeof(float));

        soundtouch::SoundTouch soundTouch;

        soundTouch.setTempoChange(tempoChange);
        soundTouch.setSampleRate(static_cast<uint32_t>(sampleRate));
        soundTouch.setChannels(channelCount);
        soundTouch.setSetting(SETTING_USE_QUICKSEEK, 1);

        buffer->getSampleBuffer()->setSize(int(inputSampleCount * soundTouch.getInputOutputSampleRatio()));

        for (int bufferIndex = 0; bufferIndex < bufferCount; bufferIndex++) {

            size_t inputStartSample = samplesPerBuffer * bufferIndex;

            const float* bufferInputChannels = inputChannels + (inputStartSample * channelCount);

            size_t outputSampleCount = changeTempo(
                    bufferInputChannels,
                    outputChannels,
                    channelCount,
                    samplesPerBuffer,
                    soundTouch,
                    bufferIndex == bufferCount - 1
                    );
            
            
            buffer->getSampleBuffer()->copyFromInterleaved(totalOutputSampleCount, outputChannels, outputSampleCount);

            totalOutputSampleCount += outputSampleCount;

            /*
             TODO: there is an inevitable race condition in this invoke
             Even if we use a OPSafeScheduler, we may try to schedule a invoke
             during the object destruction
             
             We could use a shared_ptr to prevent our OPSafeScheduler object from beign
             destroyed, but that would still allow for a invoke on a dead object
             
             But at the end of the day, we will always wait our tempo task in the
             OPMemoryPlayer destructor
             
             */
            OPCocosThread::invoke("change_tempo_buffer_processed", [onBufferProcessed, bufferIndex](){
                onBufferProcessed(bufferIndex);
            });
        }
        
        delete[] inputChannels;
        delete[] outputChannels;        
    });

    _changeTempoTask.dispatch();
}

size_t
OPMemoryPlayer::changeTempo(const float *inputChannels, float *outputChannels,
                            size_t channelCount, size_t inputSampleCount,
                            soundtouch::SoundTouch &soundTouch, bool isLastBuffer) {
    
    soundTouch.putSamples(inputChannels, inputSampleCount);

    size_t expectedSamples = inputSampleCount * soundTouch.getInputOutputSampleRatio();
    
    int samplesProcessed = soundTouch.receiveSamples(
            outputChannels,
            expectedSamples
    );

    if (isLastBuffer){
        soundTouch.flush();

        do {
            samplesProcessed += soundTouch.receiveSamples(
                    outputChannels + (samplesProcessed * channelCount),
                    expectedSamples - (samplesProcessed)
            );
        } while(soundTouch.numUnprocessedSamples() > 0);
    }

    return samplesProcessed;
}

void OPMemoryPlayer::createImplementation() {
    OPApplication::shared()->getAudioEngine()->incrementPendingImplementationCounter();
    createImplementation(_filepath, [](bool){
        OPApplication::shared()->getAudioEngine()->decrementPendingImplementationCounter();
    });
}

void OPMemoryPlayer::destroyImplementation() {
    _implementation.reset();
    _isReady = false;
}

void OPMemoryPlayer::createImplementation(const std::string &filePath,
                                          const LoadAsyncHandler &loadedCallback) {
    _implementation = std::unique_ptr<OPPlayerImplementation>(
            OPApplication::shared()->getAudioEngine()->createMemoryPlayerImplementation(
                    filePath,
                    [this, loadedCallback](bool success){
                        _isReady = success;
                        loadedCallback(success);
                    }
                    )
    );
}



