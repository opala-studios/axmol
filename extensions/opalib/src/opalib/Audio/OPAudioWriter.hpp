//
//  OPAudioWriter.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 03/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAudioWriter_hpp
#define OPAudioWriter_hpp

#include <cocos2d.h>
#include <JuceHeader.h>
#include "OPWaveformRange.hpp"
#include "OPMemoryPlayer.hpp"
#include "OPAudioFormat.hpp"

class OPAudioWriter: public cocos2d::Ref {

public:
    bool init();
    CREATE_FUNC(OPAudioWriter);
    ~OPAudioWriter();
    
    void write(OPMemoryPlayer* player, OPWaveformRange* selection,const std::string& destFilePath, OPAudioFormat::FORMAT format = OPAudioFormat::FORMAT::WAV);
    void write(const juce::AudioBuffer<float>& buffer,const std::string& destFilePath, OPAudioFormat::FORMAT format = OPAudioFormat::FORMAT::WAV);
    void write(OPAudioBuffer* audioBuffer, OPWaveformRange* selection, const std::string& destFilePath, OPAudioFormat::FORMAT audioFormat);

    
    static juce::AudioFormatWriter* getWriter(juce::OutputStream* fileStream, OPAudioFormat::FORMAT format, double sampleRate = 44100.0, int numberOfChannels = 2, int bitrate = 16);

};
#endif /* OPAudioWriter_hpp */
