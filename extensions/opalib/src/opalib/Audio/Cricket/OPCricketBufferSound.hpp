//
// Created by Ricardo on 20/10/21.
//

#ifndef OPCRICKETBUFFERSOUND_HPP
#define OPCRICKETBUFFERSOUND_HPP

#include <opalib/Audio/Cricket/OPCricketBufferSource.hpp>
#include <ck/audio/graphsound.h>

class OPCricketBufferSound : public Cki::GraphSound {
public:
    OPCricketBufferSound(std::shared_ptr<OPAudioBuffer> buffer);
    
    void play() override;
    void stop(bool tailOff = true);

    void setPlayPosition(int frame) override;
    void setPlayPositionMs(float ms) override;
private:
    OPCricketBufferSource _source;
};


#endif //OPCRICKETBUFFERSOUND_HPP
