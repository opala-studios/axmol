//
//  OPOutputRecorder.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 16/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "../Application/OPApplication.hpp"
#include "OPOutputRecorder.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
#include "OPAudioWriter.hpp"
#include "../Debug/OPDebug.hpp"

using namespace juce;


void OPOutputRecorder::startRecording (const File& file, OPAudioFormat::FORMAT format)
{
    OP_LOG("BEGIN");
    // Create an OutputStream to write to our destination file...
    file.deleteFile();
    OP_LOG_VERB("Deleted old file");
    if (auto fileStream = std::unique_ptr<FileOutputStream> (file.createOutputStream()))
    {
        OP_LOG_VERB("Created fileStream");
        int outputChannels = OPApplication::shared()->getAudioEngine()->activeOutputChannels();
        
        if (outputChannels > 2) {
            outputChannels = 2;
        }

        if(auto writer = OPAudioWriter::getWriter(fileStream.get(), format, OPApplication::shared()->getAudioEngine()->getDeviceSampleRate(), outputChannels, 16))
        {
            OP_LOG_VERB("Created writer");
            fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

            // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
            // write the data to disk on our background thread.

            threadedWriter.reset(new AudioFormatWriter::ThreadedWriter(writer, _backgroundThread, 32768));
            OP_LOG_VERB("Created threadedWriter");
            // And now, swap over our active writer pointer so that the audio callback will start using it..
            const ScopedLock sl (_writerLock);
            _activeWriter = threadedWriter.get();
        }
    }
    OP_LOG("END");
}

void OPOutputRecorder::stopRecording()
{
    OP_LOG("BEGIN");
    //First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (_writerLock);
        _activeWriter = nullptr;
    }

    // Now we can delete the writer object. It's done in this order because the deletion could
    // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
    // the audio callback while this happens.
    threadedWriter.reset();
    OP_LOG("END");
}

bool OPOutputRecorder::isRecording() const
{
    return _activeWriter.load() != nullptr;
}

bool OPOutputRecorder::startCapturing(const std::string& destFilePath, OPAudioFormat::FORMAT format)
{
    OP_LOG("BEGIN");
    OPApplication::shared()->getAudioEngine()->addAudioIOCallback(this);
    
    _backgroundThread.startThread();

    auto file = File().getChildFile(destFilePath);

    startRecording(file, format);
    OP_LOG("END");
    return true;
}

void OPOutputRecorder::stopCapturing()
{
    OP_LOG("BEGIN");
    stopRecording();
    OPApplication::shared()->getAudioEngine()->removeAudioIOCallback(this);
    //stop thread passing 1 second to guarantee any pending process
    if (!_backgroundThread.stopThread(1000)){
        OP_LOG_WARN("_backgroundThread was forcefully killed!");
    }
    OP_LOG("END");
}

OPOutputRecorder::~OPOutputRecorder()
{
    
}

OPOutputRecorder::OPOutputRecorder() : OPBaseAudioIOCallback(128){

}

void OPOutputRecorder::audioDeviceIOCallbackInt(const float **inputChannelData,
                                                int totalNumInputChannels,
                                                float **outputChannelData,
                                                int totalNumOutputChannels, int numSamples) {
    const ScopedLock sl (_writerLock);

    if (_activeWriter.load() != nullptr)
    {
        _activeWriter.load()->write (outputChannelData, numSamples);
    }
}
