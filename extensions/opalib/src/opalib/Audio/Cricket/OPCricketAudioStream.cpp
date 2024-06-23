//
// Created by Ricardo on 30/09/21.
//

#include "OPCricketAudioStream.hpp"
#include <opalib/Helpers/OPJuceHelper.hpp>
#include <opalib/Debug/OPDebug.hpp>
#include <utility>

static void interleaveSamples (const int** __restrict source, int* __restrict dest, int numSamples, int numChannels) {
    for (int chan = 0; chan < numChannels; ++chan) {
        auto i = chan;
        auto src = source[chan];

        for (int j = 0; j < numSamples; ++j) {
            dest[i] = src[j];
            i += numChannels;
        }
    }
}

static void convertInt32ToInt16(const int* src, short* dst, int numSamples){
    for (int i = 0; i < numSamples; i++){
        dst[i] = (short)src[i];
    }
}

OPCricketAudioStream::OPCricketAudioStream(std::string filepath) :
                                           _filepath(std::move(filepath)){
    OP_LOG("CONSTRUCTOR");
}

OPCricketAudioStream::~OPCricketAudioStream() {
    OP_LOG("DESTRUCTOR");
}

void OPCricketAudioStream::init() {
    _reader = OPJuceHelper::createReaderFor(_filepath);
    _inited = true;
}

bool OPCricketAudioStream::isFailed() const {
    return _inited && _reader == nullptr;
}

int OPCricketAudioStream::getChannels() const {
//    return 1;
    return (int)_reader->numChannels;
}

int OPCricketAudioStream::getSampleRate() const {
    return (int)_reader->sampleRate;
}

int OPCricketAudioStream::getLength() const {
    return _reader->lengthInSamples;
}

int OPCricketAudioStream::read(short *buf, int frames) {

    int totalSamples = getLength();
    int currentPosition = _position;
    int readSamples = 0;

    int channels = getChannels();

    const int maxSamplesPerChannel = 512;
    float floatChannel1[maxSamplesPerChannel];
    float floatChannel2[maxSamplesPerChannel];
    float* floatChannels[2] {floatChannel1, floatChannel2};
    float interleavedFloatChannels[maxSamplesPerChannel * 2];

    while (readSamples < frames && currentPosition < totalSamples){

        auto remainingFrames = frames - readSamples;
        auto remainingTotalSamples = totalSamples - currentPosition;
        auto max = std::min(std::min(remainingFrames, maxSamplesPerChannel), remainingTotalSamples);

        _reader->read(floatChannels, getChannels(), currentPosition, max);

        float* finalBuffer;
        if (channels > 1) {
            juce::AudioDataConverters::interleaveSamples(
                    const_cast<const float**>(floatChannels),
                    interleavedFloatChannels,
                    max,
                    channels
                    );
            finalBuffer = interleavedFloatChannels;
        }
        else {
            finalBuffer = floatChannel1;
        }

        juce::AudioDataConverters::convertFloatToInt16LE(finalBuffer, buf + readSamples * channels, max * channels);

        readSamples += max;
        currentPosition += max;
    }

    _position = currentPosition;

    return readSamples;
}

void OPCricketAudioStream::setPlayPosition(int frame) {
    _position = frame;
}

int OPCricketAudioStream::getPlayPosition() const {
    return _position;
}

