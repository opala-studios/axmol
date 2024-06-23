//
// Created by Ricardo on 04/10/21.
//

#include <opalib/Helpers/OPJuceHelper.hpp>
#include <ck/audio/audioformat.h>
#include <opalib/Misc/OPCocosThread.hpp>
#include <utility>
#include "OPCricketAudioBuffer.hpp"

std::shared_ptr<OPCricketAudioBuffer> OPCricketAudioBuffer::create(const std::string &filepath) {
    return std::shared_ptr<OPCricketAudioBuffer>(new OPCricketAudioBuffer(filepath));
}

OPCricketAudioBuffer::OPCricketAudioBuffer(std::string filepath) : _filepath(std::move(filepath)) {

}

void OPCricketAudioBuffer::loadAsync(OPCricketAudioBuffer::LoadCallback callback) {
    _loadCallback = std::move(callback);

    std::weak_ptr<OPCricketAudioBuffer> weakRef = shared_from_this();

    auto safeCallback = [this, weakRef](Cki::Sample* sample){
        OPCocosThread::invoke("op_sample_buffer_load_callback", [this, weakRef, sample](){
            if (weakRef.lock()){
                dispatchLoadCallback(sample);
            }
            else {
                destroySample(sample);
            }
        });
    };

    //We can't trust that "this" won't die when we first access some member
    //variable in another thread, so instead we make a copy of that member and
    //safely access it
    auto filepath = _filepath;

    auto thread = std::thread([safeCallback, filepath](){
        std::unique_ptr<juce::AudioFormatReader> reader = OPJuceHelper::createReaderFor(filepath);
        if(!reader) {
            safeCallback(nullptr);
            return;
        }

        auto buffer = std::make_unique<juce::AudioBuffer<float>>(reader->numChannels,reader->lengthInSamples);
        reader->read(
                buffer.get(),
                0,
                (int) reader->lengthInSamples,
                0,
                true,
                true
        );

        auto sample = new Cki::Sample();

        int channels = buffer->getNumChannels();
        int samples = buffer->getNumSamples();
        int dataSize = samples * channels * sizeof(float);
        sample->data = new Cki::byte[dataSize];
        sample->dataSize = dataSize;
        if (channels == 1){
            memcpy(sample->data, buffer->getReadPointer(0), dataSize);
        }
        else {
            juce::AudioDataConverters::interleaveSamples(buffer->getArrayOfReadPointers(), (float*)sample->data, samples, channels);
        }

        sample->name.set("dummy");
        sample->info.format = Cki::AudioFormat::k_pcmF32;
        sample->info.channels = channels;
        sample->info.volume = 0xFFFF;
        sample->info.pan = 0;
        sample->info.sampleRate = reader->sampleRate;
        sample->info.blocks = buffer->getNumSamples();
        sample->info.blockBytes = uint16_t(channels * sizeof(float));
        sample->info.blockFrames = 1;
        sample->info.loopStart = 0;
        sample->info.loopEnd = sample->info.blocks * sample->info.blockFrames;
        sample->info.loopCount = 0;
        safeCallback(sample);
    });

    thread.detach();
}

int OPCricketAudioBuffer::getSampleCount() const {
    return _sample->info.blocks;
}

int OPCricketAudioBuffer::getChannelCount() const {
    return _sample->info.channels;
}

const float *OPCricketAudioBuffer::getData() const {
    return (const float*)_sample->data;
}

float *OPCricketAudioBuffer::getData() {
    return (float*)_sample->data;
}

const Cki::SampleInfo &OPCricketAudioBuffer::getSampleInfo() const {
    return _sample->info;
}

void OPCricketAudioBuffer::dispatchLoadCallback(Cki::Sample* sample) {
    destroySample(_sample);
    _sample = sample;
    _loadCallback(_sample);
}

void OPCricketAudioBuffer::destroySample(Cki::Sample* sample) {
    if (sample){
        delete[] sample->data;
        delete sample;
    }
}

const Cki::Sample &OPCricketAudioBuffer::getSample() const {
    assert(_sample);
    return *_sample;
}
