//
//  OPWaveformRange.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 01/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPWaveformRange_hpp
#define OPWaveformRange_hpp

#include <stdio.h>
#include <cocos2d.h>

class OPWaveformRange: public cocos2d::Ref {

public:
    bool init();
    CREATE_FUNC(OPWaveformRange);
    ~OPWaveformRange();
    
    int getStartInSamples();
    int getEndInSamples();
    
    const cocos2d::Color4F& getColor();
    
    void setup(int startInSamples, int endInSamples);
    
    void setColor(const cocos2d::Color4F& color);
    
    bool containsIndex(int index);
    
    cocos2d::Color4F _color;
    
private:
    int _startInSamples;
    int _endInSamples;

};
#endif /* OPWaveformRange_hpp */
