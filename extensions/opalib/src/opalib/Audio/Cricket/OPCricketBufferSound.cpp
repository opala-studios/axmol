//
// Created by Ricardo on 20/10/21.
//

#include <ck/audio/audiograph.h>
#include "OPCricketBufferSound.hpp"



OPCricketBufferSound::OPCricketBufferSound(std::shared_ptr<OPAudioBuffer> buffer) :
    _source(std::move(buffer)){
    subInit(&_source);
}

void OPCricketBufferSound::play(){
    _source.prepareToPlay();
    Cki::GraphSound::play();
}

void OPCricketBufferSound::stop(bool tailOff){
    if (tailOff){
        _source.prepareToStop();
    }
    else {
        Cki::GraphSound::stop();
    }
}

void OPCricketBufferSound::setPlayPosition(int frame) {
    Cki::AudioGraph::get()->execute(&m_sourceNode, Cki::SourceNode::k_setPlayPosition, frame);
}

void OPCricketBufferSound::setPlayPositionMs(float ms) {
    Cki::AudioGraph::get()->execute(&m_sourceNode, Cki::SourceNode::k_setPlayPositionMs, ms);
}
