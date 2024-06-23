//
//  OPJuceInput.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 14/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPJuceInput.hpp"

using namespace juce;

OPJuceInput::OPJuceInput(OPAudioEngine* engine) : OPBaseAudioIOCallback(0)
{
    _engine = engine;
}

void OPJuceInput::startRendering ()
{
   _engine->addAudioIOCallback(this);
    _sampleRate = _engine->getDeviceSampleRate();
    _isRendering = true;
}

void OPJuceInput::stopRendering()
{
    _engine->removeAudioIOCallback(this);
    _sampleRate = 0;
    _isRendering = true;
}

//==============================================================================

void OPJuceInput::setRenderInputCallback(SL_RenderInputCallback renderInputCallback)
{
    _renderInputCallback = std::move(renderInputCallback);
}

void OPJuceInput::audioDeviceIOCallbackInt (const float** inputChannelData, int numInputChannels,
                            float** outputChannelData, int numOutputChannels,
                            int numSamples)
{    
    if(_renderInputCallback && numInputChannels > 0)
    {
        _renderInputCallback(inputChannelData, numSamples);
    }
    
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != nullptr)
            FloatVectorOperations::clear (outputChannelData[i], numSamples);
}

bool OPJuceInput::isRendering()
{
    return _isRendering;
}

OPJuceInput::~OPJuceInput()
{
    
}
