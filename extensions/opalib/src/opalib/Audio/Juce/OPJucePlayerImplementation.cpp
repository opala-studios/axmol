//
// Created by Ricardo on 14/10/21.
//

#include <opalib/Application/OPApplication.hpp>
#include <opalib/Debug/OPDebug.hpp>
#include "OPJucePlayerImplementation.hpp"


OPJucePlayerImplementation::OPJucePlayerImplementation(OPJuceEngine *engine) : _engine(engine){

}

OPJucePlayerImplementation::~OPJucePlayerImplementation() {
    OP_LOG("BEGIN");
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);

    const juce::MessageManagerLock mm;
    unhook();

    _player.reset(nullptr);
    _transportSource.reset(nullptr);
    _source.reset(nullptr);
    _resamplerSource.reset(nullptr);
    OP_LOG("END");
}

void OPJucePlayerImplementation::hook(){
    _engine->addAudioCallback(_player.get());
}

void OPJucePlayerImplementation::unhook(){
    _engine->removeAudioCallback(_player.get());
}

void OPJucePlayerImplementation::setup() {
    OP_LOG("BEGIN");
    const juce::MessageManagerLock mm;

    _source = createSource();
    _transportSource = std::make_unique<juce::AudioTransportSource>();
    _transportSource->setSource(_source.get());
    _resamplerSource = std::make_unique<juce::ResamplingAudioSource>(_transportSource.get(),false);
    _player = std::make_unique<juce::AudioSourcePlayer>();
    _player->setSource(_resamplerSource.get());

    setSpeed(_speed);

    hook();
    OP_LOG("END");
}

void OPJucePlayerImplementation::play() {
    _transportSource->start();
}

void OPJucePlayerImplementation::stop() {
    _transportSource->stop();
}

void OPJucePlayerImplementation::pause() {
    stop();
}

void OPJucePlayerImplementation::resume() {
    play();
}

double OPJucePlayerImplementation::getLengthInSeconds() const {
    return _transportSource->getLengthInSeconds();
}

int OPJucePlayerImplementation::getLengthInSamples() const {
    return _transportSource->getTotalLength();
}

bool OPJucePlayerImplementation::isPlaying() const {
    return _transportSource->isPlaying();
}

bool OPJucePlayerImplementation::isReady() const {
    return isPlayerReady();
}

void OPJucePlayerImplementation::setSpeed(double speed) {
    _speed = juce::jmax(0.0001, speed);

    auto deviceSampleRate = OPApplication::shared()->getAudioEngine()->getDeviceSampleRate();

    double rate = getSourceSampleRate()/ deviceSampleRate;

    _resamplerSource->setResamplingRatio(rate * _speed);
}

double OPJucePlayerImplementation::getSpeed() const {
    return _speed;
}

void OPJucePlayerImplementation::setLooping(bool shouldLoop) {
    _source->setLooping(shouldLoop);
}

bool OPJucePlayerImplementation::isLooping() const {
    return _source->isLooping();
}

void OPJucePlayerImplementation::setVolume(float gain) {
    _transportSource->setGain(gain);
}

float OPJucePlayerImplementation::getVolume() const {
    return _transportSource->getGain();
}

void OPJucePlayerImplementation::setCurrentPositionInSamples(int position) {
    if(_transportSource)
        _transportSource->setNextReadPosition(juce::int64(position));
}

int OPJucePlayerImplementation::getCurrentPositionInSamples() const {
    return _transportSource->getNextReadPosition();
}
