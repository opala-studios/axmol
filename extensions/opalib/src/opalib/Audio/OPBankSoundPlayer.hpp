//
// Created by Ricardo on 28/09/21.
//

#ifndef OPBANKSOUNDPLAYER_HPP
#define OPBANKSOUNDPLAYER_HPP

#include <ck/bank.h>
#include <ck/sound.h>

class OPBankSoundPlayer {
public:
    OPBankSoundPlayer(CkBank* bank);
    void play();
private:
    CkBank* _bank;
    CkSound* _sound;
};


#endif //OPBANKSOUNDPLAYER_HPP
