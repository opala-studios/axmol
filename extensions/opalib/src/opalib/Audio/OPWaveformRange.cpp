//
//  OPWaveformRange.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 01/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPWaveformRange.hpp"

bool OPWaveformRange::init(){
    return true;
}
OPWaveformRange::~OPWaveformRange() {
}

int OPWaveformRange::getStartInSamples()
{
    return _startInSamples;
}

int OPWaveformRange::getEndInSamples()
{
    return _endInSamples;
}

void OPWaveformRange::setup(int startInSamples, int endInSamples)
{
    _startInSamples = startInSamples;
    _endInSamples = endInSamples;
}

void OPWaveformRange::setColor(const cocos2d::Color4F& color)
{
    _color = color;
}

const cocos2d::Color4F& OPWaveformRange::getColor()
{
    return _color;
}

 bool OPWaveformRange::containsIndex(int index)
{
    if(_startInSamples > _endInSamples)
    {
        return (index >= _startInSamples || index <= _endInSamples);
    }
    
    return index >= _startInSamples && index <= _endInSamples;
}
