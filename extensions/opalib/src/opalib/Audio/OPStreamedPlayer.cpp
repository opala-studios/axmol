//
//  OPStreamedPlayer.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 09/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPStreamedPlayer.hpp"
#include <opalib/Audio/OPAudioEngine.hpp>
#include <opalib/Debug/OPDebug.hpp>

std::unique_ptr<OPStreamedPlayer> OPStreamedPlayer::createUnique(const std::string& filePath)
{
    return std::make_unique<OPStreamedPlayer>(filePath);
}

std::shared_ptr<OPStreamedPlayer> OPStreamedPlayer::createShared(const std::string& filePath)
{
    return std::make_shared<OPStreamedPlayer>(filePath);
}

OPStreamedPlayer::OPStreamedPlayer(const std::string& filePath) : _filepath(filePath) {
    OP_LOG("BEGIN");
    OPStreamedPlayer::createImplementation();
    OP_LOG("END");
}

void OPStreamedPlayer::createImplementation() {
    _implementation = std::unique_ptr<OPPlayerImplementation>(OPApplication::shared()->getAudioEngine()->createStreamedPlayerImplementation(_filepath));
}

void OPStreamedPlayer::destroyImplementation() {
    _implementation.reset();
}
