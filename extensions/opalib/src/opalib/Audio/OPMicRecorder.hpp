//
//  OPMicRecorder.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 16/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPMicRecorder_hpp
#define OPMicRecorder_hpp

#include "./OPJuceInput.hpp"
#include "./OPAudioFormat.hpp"

class OPMicRecorder: public OPJuceInput {
public:
    void setRenderInputMicCallback(SL_RenderInputCallback renderInputMicCallback);

    OPMicRecorder(OPAudioEngine* engine);
    ~OPMicRecorder();
    bool startCapturing(const std::string& destFilePath, OPAudioFormat::FORMAT format = OPAudioFormat::FORMAT::WAV);
    void stopCapturing();
    
    bool isRecording() const;

    void update(float dt);

private:
    SL_RenderInputCallback _renderInputMicCallback;
    // the FIFO used to buffer the incoming data
    juce::CriticalSection _writerLock;
    // the pointer used to store the writer
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> _threadedWriter{nullptr};
    // the writer used on the sound thread
    std::atomic<juce::AudioFormatWriter::ThreadedWriter*> _activeWriter { nullptr };
    // Buffer used to draw
    juce::AudioBuffer<float> _buffer;
    // the thread that will write our audio data to disk
    juce::TimeSliceThread _backgroundThread { "InputMicRecorderThread" };

    //Padding for safety reasons, see OPOutputRecorder
    uint64_t _padding = 1;

    void startRecording (const juce::File& file, OPAudioFormat::FORMAT format);
    void stop();
};
#endif /* OPMicRecorder_hpp */
