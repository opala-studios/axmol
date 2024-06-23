//
// Created by Ricardo on 05/10/21.
//

#include "OPCricketSynthesizerImplementation.hpp"
#include <opalib/Debug/OPDebug.hpp>

OPCricketSynthesizerImplementation::OPCricketSynthesizerImplementation(int numberOfVoices) {
    OP_LOG("BEGIN");
    _voices.resize(numberOfVoices);
    for(auto& voice : _voices){
        voice.player = std::make_shared<OPCricketBufferPlayer>();
    }
    OP_LOG("END");
}

OPCricketSynthesizerImplementation::~OPCricketSynthesizerImplementation() {
    OP_LOG("BEGIN");
    _voices.clear();
    unloadAll();
    OP_LOG("END");
}

bool OPCricketSynthesizerImplementation::findAvailableVoice(size_t &availableVoiceIndex) {
    for(size_t i = 0; i < _voices.size(); i++){
        if (!_voices[i].player->isPlaying()){
            availableVoiceIndex = i;
            return true;
        }
    }
    return false;
}

bool OPCricketSynthesizerImplementation::findVoicePlayingItem(OPSynthItem *item, size_t &voiceIndex){
    for(size_t i = 0; i < _voices.size(); i++){
        auto& voice = _voices[i];
        if (voice.player->isPlaying() && voice.item == item){
            voiceIndex = i;
            return true;
        }
    }
    return false;
}

void OPCricketSynthesizerImplementation::play(OPSynthItem *item) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    if (!_itemToSampler[item].audioBuffer->isLoaded()){
        return;
    }
    size_t voiceIndex;
    if (findAvailableVoice(voiceIndex)){
        stop(item);
        auto& voice = _voices[voiceIndex];
        auto& sampler = _itemToSampler[item];
        voice.player->setAudioBuffer(sampler.audioBuffer);
        voice.player->play();
        voice.player->setVolume(sampler.volume);
        voice.player->setSpeed(sampler.speed);
        voice.player->setLoop(sampler.looping);
        voice.item = item;
    }
}

void OPCricketSynthesizerImplementation::stop(OPSynthItem *item) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    size_t voiceIndex;
    if (findVoicePlayingItem(item, voiceIndex)){
        auto& voice = _voices[voiceIndex];
        voice.player->stop();
        voice.item = nullptr;
    }
}

void OPCricketSynthesizerImplementation::loadItem(OPSynthItem *item,
                                                  const OPSynthesizerImplementation::LoadItemHandler &handler) {
    OP_LOG("BEGIN");
    if (_itemToSampler.find(item) != _itemToSampler.end()){
        OP_LOG("Item was already loaded: " + item->getFilePath());
        if (handler){
            handler(true);
        }
        return;//item was already added, so we just return
    }
    _itemToSampler[item].audioBuffer = OPAudioBuffer::create(item->getFilePath(), OPChannelLayout::INTERLEAVED);
    _itemToSampler[item].looping = item->isLoop();
    _itemToSampler[item].audioBuffer->loadAsync([this, item, handler](bool success){
        OP_LOG("Load item result - " + std::to_string(success));
        if (!success) {
            _itemToSampler.erase(item);
        }
        if (handler){
            handler(success);
        }
    });
    OP_LOG("END");
}

bool OPCricketSynthesizerImplementation::isLoaded(OPSynthItem* item) const {
    return _itemToSampler.find(item) != _itemToSampler.end() && _itemToSampler.at(item).audioBuffer->isLoaded();
}

void OPCricketSynthesizerImplementation::setIsLooping(OPSynthItem* item, bool looping) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item].looping = looping;
    updateVoice(item);
}

bool OPCricketSynthesizerImplementation::getIsLooping(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item).looping;
}

void OPCricketSynthesizerImplementation::setSpeed(float speed) {
    for (auto& kv : _itemToSampler){
        auto& sampler = kv.second;
        sampler.speed = speed;
    }
    updateAllVoices();
}

void OPCricketSynthesizerImplementation::setSpeed(OPSynthItem* item, float speed) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item].speed = speed;
    updateVoice(item);
}

float OPCricketSynthesizerImplementation::getSpeed(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item).speed;
}

void OPCricketSynthesizerImplementation::setVolume(float volume) {
    for (auto& kv : _itemToSampler){
        auto& sampler = kv.second;
        sampler.volume = volume;
    }
    updateAllVoices();
}

void OPCricketSynthesizerImplementation::setVolume(OPSynthItem* item, float volume) {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    _itemToSampler[item].volume = volume;
    updateVoice(item);
}

float OPCricketSynthesizerImplementation::getVolume(OPSynthItem* item) const {
    assert(_itemToSampler.find(item) != _itemToSampler.end());
    return _itemToSampler.at(item).volume;
}

// TODO: We probably should implement a proper voice player
// that reads our speed and volume variables when rendering
// audio so that we don't need to make this ugly update
void OPCricketSynthesizerImplementation::updateVoice(OPSynthItem* item){
    size_t voiceIndex;
    if (findVoicePlayingItem(item, voiceIndex)){
        auto& sampler = _itemToSampler[item];
        auto& voice = _voices[voiceIndex];
        voice.player->setSpeed(sampler.speed);
        voice.player->setVolume(sampler.volume);
        voice.player->setLoop(sampler.looping);
    }
}

void OPCricketSynthesizerImplementation::updateAllVoices(){
    for (auto& voice : _voices){
        if (voice.player->isPlaying()){
            auto& sampler = _itemToSampler[voice.item];
            voice.player->setVolume(sampler.volume);
            voice.player->setSpeed(sampler.speed);
            voice.player->setLoop(sampler.looping);
        }
    }
}

void OPCricketSynthesizerImplementation::unloadItem(OPSynthItem *item) {
    OP_LOG("BEGIN");
    stop(item);
    _itemToSampler.erase(item);
    OP_LOG("END");
}

void OPCricketSynthesizerImplementation::unloadAll() {
    OP_LOG("BEGIN");
    for (auto& kv : _itemToSampler){
        stop(kv.first);
    }
    _itemToSampler.clear();
    OP_LOG("END");
}

