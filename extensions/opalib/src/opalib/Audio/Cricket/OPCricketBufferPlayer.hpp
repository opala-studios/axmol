//
// Created by Ricardo on 01/10/21.
//

#ifndef OPCRICKETBUFFERPLAYER_HPP
#define OPCRICKETBUFFERPLAYER_HPP

#include <string>
#include <opalib/Audio/OPAudioBuffer.hpp>
#include "OPCricketBufferSound.hpp"

class OPCricketBufferPlayer {
public:
    typedef std::function<void(bool)> LoadAsyncCallback;

public:
    ~OPCricketBufferPlayer();
    void setAudioBuffer(std::shared_ptr<OPAudioBuffer> sampleBuffer);

    void play();
    void stop();
    void setSpeed(float speed);
    void setVolume(float volume);
    void setLoop(bool loop);

    bool isPlaying();

private:
    OPCricketBufferSound* _sound;
    std::shared_ptr<OPAudioBuffer> _audioBuffer;
};


#endif //OPCRICKETBUFFERPLAYER_HPP
