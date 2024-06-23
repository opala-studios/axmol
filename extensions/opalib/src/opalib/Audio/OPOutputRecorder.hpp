//
//  OPOutputRecorder.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 16/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPOutputRecorder_hpp
#define OPOutputRecorder_hpp

#include "./OPJuceInput.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "./OPAudioFormat.hpp"

class OPOutputRecorder : OPBaseAudioIOCallback
{
public:
    OPOutputRecorder();
    ~OPOutputRecorder();
    bool startCapturing(const std::string& destFilePath, OPAudioFormat::FORMAT format = OPAudioFormat::FORMAT::WAV);
    void stopCapturing();
    
    bool isRecording() const;

private:
    void audioDeviceIOCallbackInt(const float **inputChannelData, int totalNumInputChannels,
                                  float **outputChannelData, int totalNumOutputChannels,
                                  int numSamples) override;

private:

    // the thread that will write our audio data to disk
    juce::TimeSliceThread _backgroundThread { "OutputRecorderThread" };

    //Add 8 bytes of padding for safety measures, for some reason the 4 bytes after _backgroundThread
    //are set to 0 as soon as we construct our threadedWriter
    uint64_t _padding = 1;

    // the FIFO used to buffer the incoming data
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> threadedWriter {nullptr};
    juce::CriticalSection _writerLock;
    std::atomic<juce::AudioFormatWriter::ThreadedWriter*> _activeWriter { nullptr };

    void startRecording (const juce::File& file, OPAudioFormat::FORMAT format);
    void stopRecording();
};
#endif /* OPOutputRecorder_hpp */
