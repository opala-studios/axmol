//
// Created by Ricardo on 28/09/21.
//

#include "OPBankSoundPlayer.hpp"

OPBankSoundPlayer::OPBankSoundPlayer(CkBank *bank) : _bank(bank) {
    _sound = CkSound::newBankSound(bank, 0);
    _sound->setLoopCount(0);
}

void OPBankSoundPlayer::play() {
    _sound->play();
}
