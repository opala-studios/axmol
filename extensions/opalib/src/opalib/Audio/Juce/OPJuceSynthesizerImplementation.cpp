//
// Created by Ricardo on 13/10/21.
//

#include <cocos2d/cocos/base/CCAsyncTaskPool.h>
#include <opalib/Helpers/OPJuceHelper.hpp>
#include <opalib/Debug/OPDebug.hpp>
#include "OPJuceSynthesizerImplementation.hpp"

OPJuceSynthesizerImplementation::OPJuceSynthesizerImplementation(OPJuceEngine* engine) : _engine(engine) {
    OP_LOG("BEGIN");
    _synth = std::make_unique<juce::Synthesiser>();

    _midiCollector.reset(new juce::MidiMessageCollector());

    for(int i = 0; i < 16; i++){
        _synth->addVoice(new OPSamplerVoice());
    }

    _engine->addAudioCallback(this);
    OP_LOG("END");
}

OPJuceSynthesizerImplementation::~OPJuceSynthesizerImplementation() {
    _engine->removeAudioCallback(this);
    _synth->clearVoices();
    _synth->clearSounds();
}

void OPJuceSynthesizerImplementation::play(OPSynthItem *item) {
    auto message = juce::MidiMessage::noteOn(item->getPage(), item->getNote(), juce::uint8(127));
    message.addToTimeStamp(juce::Time::getMillisecondCounterHiRes());
    _midiCollector->addMessageToQueue(message);
}

void OPJuceSynthesizerImplementation::stop(OPSynthItem *item) {
    auto message = juce::MidiMessage::noteOff(item->getPage(), item->getNote(), juce::uint8(127));
    message.addToTimeStamp(juce::Time::getMillisecondCounterHiRes());
    _midiCollector->addMessageToQueue(message);
}

void OPJuceSynthesizerImplementation::loadItem(OPSynthItem *item,
                                               const OPSynthesizerImplementation::LoadItemHandler &handler) {
    cocos2d::AsyncTaskPool::getInstance()->enqueue(
            cocos2d::AsyncTaskPool::TaskType::TASK_IO,
            [this, handler,item](void*) {
                if(handler && item){
                    handler(_itemToSampler.find(item) != _itemToSampler.end());
                }
            },
            nullptr,
            [this, item]() {
                std::unique_ptr<juce::AudioFormatReader> reader = OPJuceHelper::createReaderFor(item->getFilePath());

                if (!reader) {
                    OPDebug::log("OPSynthesizer", "fail to load: " + item->getFilePath(), true);
                    return;
                }

                juce::BigInteger notesRange;
                notesRange.setRange (item->getNote(), 1, true);

                auto sound = std::make_unique<OPSamplerSound>("",
                                                              *reader,
                                                              notesRange,
                                                              item->getNote(),
                                                              256 / reader->sampleRate,
                                                              256 / reader->sampleRate,
                                                              reader->lengthInSamples / reader->sampleRate,
                                                              item->getPage());
                if (_synth && item) {
                    sound->setLoop(item->isLoop());
                    _synth->addSound(sound.get());
                    _itemToSampler.emplace(item, sound.get());
                    sound.release();
                    OPDebug::log("OPSynthesizer", "loaded: " + item->getFilePath(), true);
                }
            });
}

void OPJuceSynthesizerImplementation::unloadItem(OPSynthItem *item) {
    auto sound = _itemToSampler[item];
    for(int i = 0; i <= _synth->getNumSounds() - 1; i++) {
        if(_synth->getSound(i) == sound) {
            _synth->removeSound(i);
            _itemToSampler.erase(item);
            break;
        }
    }
}

void OPJuceSynthesizerImplementation::unloadAll() {
    _synth->clearSounds();
    _itemToSampler.clear();
}


bool OPJuceSynthesizerImplementation::isLoaded(OPSynthItem *item) const {
    return _itemToSampler.find(item) != _itemToSampler.end();
}


void OPJuceSynthesizerImplementation::setSpeed(float speed) {
    for (auto& kv : _itemToSampler){
        auto sampler = kv.second;
        sampler->setSpeed(speed);
    }
}

void OPJuceSynthesizerImplementation::setSpeed(OPSynthItem* item, float speed) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item]->setSpeed(speed);
}

float OPJuceSynthesizerImplementation::getSpeed(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item)->getVolume();
}

void OPJuceSynthesizerImplementation::setVolume(float volume) {
    for (auto& kv : _itemToSampler){
        auto sampler = kv.second;
        sampler->setVolume(volume);
    }
}

void OPJuceSynthesizerImplementation::setVolume(OPSynthItem* item, float volume) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item]->setVolume(volume);
}

float OPJuceSynthesizerImplementation::getVolume(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item)->getSpeed();
}

void OPJuceSynthesizerImplementation::setIsLooping(OPSynthItem* item, bool looping) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item]->setLoop(looping);
}

bool OPJuceSynthesizerImplementation::getIsLooping(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item)->getIsLooping();
}

void OPJuceSynthesizerImplementation::audioDeviceIOCallback(const float **inputChannelData,
                                                            int numInputChannels,
                                                            float **outputChannelData,
                                                            int numOutputChannels, int numSamples) {
    juce::MidiBuffer incomingMidi;

    _midiCollector->removeNextBlockOfMessages(incomingMidi, numSamples);

    juce::AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);
    buffer.clear();

    _synth->renderNextBlock(buffer, incomingMidi, 0, numSamples);
}

void OPJuceSynthesizerImplementation::audioDeviceAboutToStart(juce::AudioIODevice *device) {
    OP_LOG("BEGIN");
    _midiCollector->reset(device->getCurrentSampleRate());
    _synth->setCurrentPlaybackSampleRate(device->getCurrentSampleRate());
    OP_LOG("END");
}

void OPJuceSynthesizerImplementation::audioDeviceStopped() {
    OP_LOG("STOPPED");
}
