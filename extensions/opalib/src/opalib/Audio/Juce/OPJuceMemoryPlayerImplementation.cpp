//
// Created by Ricardo on 18/10/21.
//

#include <opalib/Audio/OPMemoryAudioSource.hpp>
#include "OPJuceMemoryPlayerImplementation.hpp"

OPJuceMemoryPlayerImplementation::OPJuceMemoryPlayerImplementation(OPJuceEngine *engine,
                                                                   const std::string &filepath,
                                                                   LoadAsyncHandler callback) :
                                                                   OPJucePlayerImplementation(engine){
    _loadedCallback = std::move(callback);

    _audioBuffer = OPAudioBuffer::create(filepath, OPChannelLayout::SEPARATED);

    _audioBuffer->loadAsync([&](bool success){
        if(success){
            setup();
        }
        _loadedCallback(success);
    });
}

OPJuceMemoryPlayerImplementation::~OPJuceMemoryPlayerImplementation(){
    // If we don't unhook here, our audio buffer may be used in
    // our audio callback during this object destruction, causing an explosion
    unhook();
}

double OPJuceMemoryPlayerImplementation::getSourceSampleRate() {
    return _audioBuffer->getSampleRate();
}

bool OPJuceMemoryPlayerImplementation::isPlayerReady() const {
    return _audioBuffer->isLoaded();
}

std::unique_ptr<juce::PositionableAudioSource> OPJuceMemoryPlayerImplementation::createSource() {
    juce::AudioBuffer<float> buffer(_audioBuffer->getSampleBuffer()->getSeparatedChannelPointerArray(),
                                    _audioBuffer->getChannelCount(), _audioBuffer->getSampleCount());
    auto source = std::make_unique<OPMemoryAudioSource>(std::move(buffer));
    return std::move(source);
}

std::shared_ptr<OPAudioBuffer> OPJuceMemoryPlayerImplementation::getAudioBuffer() {
    return _audioBuffer;
}
