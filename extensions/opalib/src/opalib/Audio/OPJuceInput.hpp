//
//  OPJuceInput.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 14/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPJuceInput_hpp
#define OPJuceInput_hpp

#include <JuceHeader.h>
#include "OPAudioEngine.hpp"

class OPJuceInput : public OPBaseAudioIOCallback
{
public:
    typedef std::function<void(const float**, int numberOfSamples)> SL_RenderInputCallback;
    
private:
    
    bool _isRendering = false;
    
protected:    
    double _sampleRate = 0.0;

    void audioDeviceIOCallbackInt (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override;
    
    SL_RenderInputCallback _renderInputCallback;
    OPAudioEngine* _engine = nullptr;
    size_t _callbackId = 0;

public:
    void setRenderInputCallback(SL_RenderInputCallback renderInputCallback);

    OPJuceInput(OPAudioEngine* engine);
    ~OPJuceInput();
    
    virtual void startRendering();
    virtual void stopRendering();
    virtual bool isRendering();
};
#endif /* OPJuceInput_hpp */
