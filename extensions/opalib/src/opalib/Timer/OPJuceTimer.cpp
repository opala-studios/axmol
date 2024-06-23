//
//  OPJuceTimer.cpp
//  opalib-ios
//
//  Created by Arthur Abel Motelevicz on 27/05/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//


#include "OPJuceTimer.hpp"


OPJuceTimer::OPJuceTimer()
{}

OPJuceTimer::~OPJuceTimer()
{
    stop();
}

void OPJuceTimer::setTimerCallback(float dt, SL_TimerCallback callback)
{
    _deltaTime = dt;
    _callback = callback;
}

void OPJuceTimer::stop(){
    stopTimer();
}
void OPJuceTimer::start()
{
    startTimer(_deltaTime);
}

void OPJuceTimer::timerCallback()
{
    if(_callback)
        _callback();
}
