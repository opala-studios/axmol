//
//  OPJuceTimer.hpp
//  opalib-ios
//
//  Created by Arthur Abel Motelevicz on 27/05/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPJuceTimer_hpp
#define OPJuceTimer_hpp


#include <JuceHeader.h>

class OPJuceTimer: juce::Timer
{
public:
    typedef std::function<void()> SL_TimerCallback;
    OPJuceTimer();
    ~OPJuceTimer();
    
    void setTimerCallback(float dt, SL_TimerCallback);
    
    void stop();
    void start();
    
protected:
    SL_TimerCallback _callback;
   
    void timerCallback() override;
    float _deltaTime{1.0f}; //default 1 ms
    
};

#endif /* OPJuceTimer_hpp */
