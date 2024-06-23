//
//  OPAudio.hpp
//  OpalaApp-mobile
//
//  Created by Adilson Tavares on 05/08/19.
//

#ifndef OPAudio_hpp
#define OPAudio_hpp

#include <iostream>
#include <map>
#include <cocos2d.h>

class OPSoundEffects
{
private:

    bool initialized {false};

protected:

//    cocos2d::Map<std::string, OPInstrumentItem*> items;
//    OPInstrument* instrument;
    
    OPSoundEffects();

public:

    ~OPSoundEffects();
    
    void initialize();

    static OPSoundEffects* shared();
    
    virtual bool play(const std::string& name);
    virtual bool stop(const std::string& name);
};

#endif /* OPAudio_hpp */
