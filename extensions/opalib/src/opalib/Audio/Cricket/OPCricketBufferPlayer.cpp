//
// Created by Ricardo on 01/10/21.
//

#include "OPCricketBufferPlayer.hpp"
#include <ck/audio/audiograph.h>
#include <ck/audio/audioformat.h>
#include <opalib/Debug/OPDebug.hpp>

void OPCricketBufferPlayer::play() {
    if (_sound){
        _sound->play();
    }
}

void OPCricketBufferPlayer::stop() {
    if (_sound){
        _sound->stop();
    }
}

void OPCricketBufferPlayer::setSpeed(float speed) {
    if (_sound){
        _sound->setSpeed(speed);
    }
}

void OPCricketBufferPlayer::setVolume(float volume) {
    if (_sound){
        _sound->setVolume(volume);
    }
}

void OPCricketBufferPlayer::setLoop(bool loop){
    if (_sound){
        _sound->setLoopCount(loop ? -1 : 0);
    }
}

void OPCricketBufferPlayer::setAudioBuffer(std::shared_ptr<OPAudioBuffer> sampleBuffer) {
    if (_sound){
        _sound->destroy();
        _sound = nullptr;
    }

    _audioBuffer = std::move(sampleBuffer);
    if (_audioBuffer){
        _sound = new OPCricketBufferSound(_audioBuffer);
    }
}

bool OPCricketBufferPlayer::isPlaying() {
    return _sound && _sound->isPlaying();
}

OPCricketBufferPlayer::~OPCricketBufferPlayer() {
    OP_LOG("BEGIN");
    if (_sound){
        _sound->destroy();
        _sound = nullptr;
    }
    OP_LOG("END");
}
