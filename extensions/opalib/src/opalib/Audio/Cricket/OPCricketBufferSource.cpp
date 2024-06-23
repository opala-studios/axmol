//
// Created by Ricardo on 20/10/21.
//

#include <ck/audio/audioformat.h>
#include "OPCricketBufferSource.hpp"
#include <opalib/Debug/OPDebug.hpp>

OPCricketBufferSource::OPCricketBufferSource() = default;

OPCricketBufferSource::OPCricketBufferSource(std::shared_ptr<OPAudioBuffer> buffer) {
    OP_LOG("BEGIN");
    setAudioBuffer(buffer);
    OP_LOG("END");
}

OPCricketBufferSource::~OPCricketBufferSource() {
    OP_LOG("DESTRUCTOR");
    
}

void OPCricketBufferSource::setAudioBuffer(std::shared_ptr<OPAudioBuffer> buffer) {
    assert(buffer->getSampleBuffer()->getChannelLayout() == OPChannelLayout::INTERLEAVED);
    _buffer = buffer;
    auto sampleRate = buffer->getSampleRate();
    auto adsrParameters = juce::ADSR::Parameters{};
    adsrParameters.attack = 256 / sampleRate;
    adsrParameters.release = 256 / sampleRate;
    
    _adsr.setSampleRate(sampleRate);
    _adsr.setParameters(adsrParameters);
    
    _sampleInfo.format = Cki::AudioFormat::k_pcmF32;
    _sampleInfo.sampleRate = sampleRate;
    _sampleInfo.channels = buffer->getChannelCount();
    _sampleInfo.pan = 0;
    _sampleInfo.blocks = buffer->getSampleCount();
    _sampleInfo.blockBytes = uint16_t(buffer->getChannelCount() * sizeof(float));
    _sampleInfo.blockFrames = 1;
    _sampleInfo.volume = 0xFFFF;
    _sampleInfo.loopStart = 0;
    _sampleInfo.loopEnd = _sampleInfo.blocks * _sampleInfo.blockFrames;
    _sampleInfo.loopCount = 0;
}

void OPCricketBufferSource::prepareToPlay(){
    _adsr.noteOn();
}

void OPCricketBufferSource::prepareToStop(){
    _adsr.noteOff();
}

int OPCricketBufferSource::read(void *buf, int blocks) {
    if (!_buffer){
        return 0;
    }

    auto dstPtr = (float*)buf;
    auto max = 0, dstPos = 0;
    auto srcSampleCount = _sampleInfo.blocks, dstSampleCount = blocks;
    
    auto channels = _buffer->getChannelCount();

    auto src = _buffer->getSampleBuffer();

    int srcPos = _position;
    while ((srcPos < srcSampleCount || isLooping()) && (dstPos < dstSampleCount)) {
        max = juce::jmin(dstSampleCount - dstPos, srcSampleCount - (srcPos % srcSampleCount));
        
        auto startDstPtr = dstPtr + (dstPos * channels);

        src->copyToInterleaved(srcPos, startDstPtr, max);
        
        applyEnvelopeToBuffer(startDstPtr, channels, max);
        
        dstPos += max;
        srcPos += max;
        if (srcPos == srcSampleCount && isLooping()){
            _loopCurrent++;
            srcPos = 0;
        }
    }

    _position = isLooping() ? (srcPos % srcSampleCount) : srcPos;

    return dstPos;
}

//Will apply adsr value to each sample of the specified buffer
void OPCricketBufferSource::applyEnvelopeToBuffer(float* buffer, int channelCount, int sampleCount){
    
    for (int i = 0; i < sampleCount; i++){
        auto envelopeValue = _adsr.getNextSample();
        for (int ch = 0; ch < channelCount; ch++){
            buffer[(i * channelCount) + ch] *= envelopeValue;
        }
    }
}

int OPCricketBufferSource::getNumBlocks() const {
    return _sampleInfo.blocks;
}

void OPCricketBufferSource::setBlockPos(int block) {
    _position = block;
}

int OPCricketBufferSource::getBlockPos() const {
    return _position;
}

void OPCricketBufferSource::reset() {
    _position = 0;
    _loopCurrent = 0;
    _releaseLoop = false;
    _adsr.reset();
}

const Cki::SampleInfo &OPCricketBufferSource::getSampleInfo() const {
    return _sampleInfo;
}

bool OPCricketBufferSource::isInited() const {
    return true;
}

bool OPCricketBufferSource::isReady() const {
    return _buffer && _buffer->isLoaded();
}

bool OPCricketBufferSource::isFailed() const {
    return false;
}

bool OPCricketBufferSource::isDone() const {
    if (!_buffer){
        return true;
    }
    bool looping = isLooping();
    return (!looping && _position == _buffer->getSampleCount()) || !_adsr.isActive();
}

void OPCricketBufferSource::setLoop(int startFrame, int endFrame) {
    _sampleInfo.loopStart = startFrame;
    _sampleInfo.loopEnd = endFrame;
}

void OPCricketBufferSource::getLoop(int &startFrame, int &endFrame) const {
    startFrame = _sampleInfo.loopStart;
    endFrame = _sampleInfo.loopEnd;
}

void OPCricketBufferSource::setLoopCount(int i) {
    _sampleInfo.loopCount = i;
}

int OPCricketBufferSource::getLoopCount() const {
    return _sampleInfo.loopCount;
}

int OPCricketBufferSource::getCurrentLoop() const {
    return _loopCurrent;
}

void OPCricketBufferSource::releaseLoop() {
    _releaseLoop = true;
}

bool OPCricketBufferSource::isLoopReleased() const {
    return _releaseLoop;
}

bool OPCricketBufferSource::isLooping() const {
    return !_releaseLoop && (_sampleInfo.loopCount < 0 || _loopCurrent < _sampleInfo.loopCount);
}
