//
//  OPMicRecorder.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 16/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPMicRecorder.hpp"
#include "OPAudioWriter.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"
using namespace juce;

OPMicRecorder::OPMicRecorder(OPAudioEngine* manager):OPJuceInput(manager)
{
    setRenderInputCallback([this](const float** incomingData, int numberOfSamples)
    {
        const ScopedLock sl (_writerLock);

        if (_activeWriter.load() != nullptr)
        {
            _activeWriter.load()->write(incomingData, numberOfSamples);
        }

        if (_renderInputMicCallback)
        {
            _buffer.setSize(1, numberOfSamples);
            _buffer.copyFrom(0, 0, incomingData[0], numberOfSamples);
        }
    });
}

void OPMicRecorder::startRecording (const File& file, OPAudioFormat::FORMAT format)
{
    stop();

    if (_sampleRate > 0)
    {
        // Create an OutputStream to write to our destination file...
        file.deleteFile();

        if (auto fileStream = std::unique_ptr<FileOutputStream> (file.createOutputStream()))
        {
            if(auto writer = OPAudioWriter::getWriter(fileStream.get(), format, _sampleRate, 1, 16))
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                _threadedWriter.reset (new AudioFormatWriter::ThreadedWriter (writer, _backgroundThread, 32768));

                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (_writerLock);
                _activeWriter = _threadedWriter.get();
            }
        }
    }
}

void OPMicRecorder::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (_writerLock);
        _activeWriter = nullptr;
    }

    // Now we can delete the writer object. It's done in this order because the deletion could
    // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
    // the audio callback while this happens.
    _threadedWriter.reset();

    _buffer = AudioBuffer<float>();
}

bool OPMicRecorder::isRecording() const
{
    return _activeWriter.load() != nullptr;
}

void OPMicRecorder::update(float dt)
{
    if (_renderInputMicCallback && _buffer.getNumSamples() > 0) {
        AudioBuffer<float> buffer;
        buffer.makeCopyOf(_buffer);
        _renderInputMicCallback(buffer.getArrayOfReadPointers(), buffer.getNumSamples());
    }
}

bool OPMicRecorder::startCapturing(const std::string& destFilePath, OPAudioFormat::FORMAT format)
{
    if (_renderInputMicCallback) {
        cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    }

    startRendering();
    _backgroundThread.startThread();

    auto file = File().getChildFile(destFilePath);

    startRecording(file, format);
    return true;
}

void OPMicRecorder::setRenderInputMicCallback(SL_RenderInputCallback renderInputMicCallback)
{
    _renderInputMicCallback = renderInputMicCallback;
}

void OPMicRecorder::stopCapturing()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    stop();

    stopRendering();
    //stop thread passing 1 second to guarantee any pending process
    _backgroundThread.stopThread(1000);
}

OPMicRecorder::~OPMicRecorder()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}
