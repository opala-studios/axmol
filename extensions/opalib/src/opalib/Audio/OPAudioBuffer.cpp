//
//  OPAudioBuffer.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 17/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAudioBuffer.hpp"
#include "../Datetime/OPTimer.hpp"
#include "../Debug/OPDebug.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"

#include <Misc/OPDirectory.hpp>
#include <Helpers/OPJuceHelper.hpp>
#include <Crash/OPCrashAnalytics.hpp>
#include <utility>

using namespace juce;

std::shared_ptr<OPAudioBuffer>
OPAudioBuffer::create(const std::string &filePath, OPAudioBuffer::ChannelLayout channelLayout) {
    return std::shared_ptr<OPAudioBuffer> (new OPAudioBuffer(filePath, channelLayout));
}

OPAudioBuffer::OPAudioBuffer(std::string filePath, ChannelLayout channelLayout) :
    _filePath(std::move(filePath)), _buffer(std::make_unique<OPSampleBuffer>(channelLayout)) {

}

bool OPAudioBuffer::isLoaded() {
    return _buffer->isAllocated();
}

bool OPAudioBuffer::isLoading(){
    return _loading;
}

double OPAudioBuffer::getSampleRate(){
    return _reader->sampleRate;
}

double OPAudioBuffer::getLenghtInSeconds(){
    return _reader->lengthInSamples / _reader->sampleRate;
}

void OPAudioBuffer::loadAsync(const LoadAsyncHandler& completion){
    _loading = true;
    
    OPAudioLoader::loadBufferAsync( _filePath, _buffer->getChannelLayout(),
                                    shared_from_this(),
                                    [&,completion](std::unique_ptr<OPSampleBuffer> buffer,
                                                   std::unique_ptr<juce::AudioFormatReader> reader){
        _loading = false;
        if(!buffer){

            std::stringstream ss;
            ss << "OPAudioBuffer failed to load audio at path: " << _filePath;
            OPCrashAnalytics::shared()->recordException(ss.str());

            if (completion){
                completion(false);
            }
            return;
        }
        
        _buffer = std::move(buffer);
        _reader  = std::move(reader);

        if (completion) {
            completion(true);
        }
    });
}

bool OPAudioBuffer::loadSync() {
    _loading = true;

    OPAudioLoader::loadBufferSync(_filePath, _buffer->getChannelLayout(),
                                  [this](std::unique_ptr<OPSampleBuffer> buffer,
                                                    std::unique_ptr<juce::AudioFormatReader> reader){
        if(!buffer) {
            std::stringstream ss;
            ss << "OPAudioBuffer failed to load audio at path: " << _filePath;
            OPCrashAnalytics::shared()->recordException(ss.str());
            return;
        }
        _buffer = std::move(buffer);
        _reader = std::move(reader);
    });
    _loading = false;
    return isLoaded();
}

void OPAudioBuffer::unload() {
    _buffer->clear();
}

const std::string& OPAudioBuffer::getFilePath() const {
    return _filePath;
}

OPAudioBuffer::~OPAudioBuffer() {
    _buffer.reset();
}

int OPAudioBuffer::getSampleCount() const {
    return _buffer->getSampleCount();
}

int OPAudioBuffer::getChannelCount() const {
    return _buffer->getChannelCount();
}

OPAudioBuffer::SampleBuffer* OPAudioBuffer::getSampleBuffer() {
    return _buffer.get();
}

//MARK: OPAudioLoader
void OPAudioLoader::loadBufferAsync(const std::string& filePath,
                                    OPChannelLayout finalLayout,
                                    const std::weak_ptr<OPAudioBuffer>& owner,
                                    const LoadHandler& completion) {

    auto thread = std::thread([filePath, finalLayout, owner, completion]() {
      
        loadBufferSync(filePath, finalLayout, [owner, completion](
                std::unique_ptr<OPSampleBuffer> buffer,
                std::unique_ptr<juce::AudioFormatReader> reader){
            auto readerPtr = reader.release();
            auto bufferPtr = buffer.release();

            OPCocosThread::invoke("op_audio_buffer_load_completion",
            [owner, completion, readerPtr, bufferPtr]{
                std::unique_ptr<OPSampleBuffer> buffer(bufferPtr);
                std::unique_ptr<juce::AudioFormatReader> reader(readerPtr);

                if(owner.lock() && completion) {
                    completion(std::move(buffer), std::move(reader));
                }
            });

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            cocos2d::JniHelper::getJavaVM()->DetachCurrentThread();
#endif
        });
    });
   
    thread.detach();
}

void OPAudioLoader::loadBufferSync(const std::string &filepath,
                                   OPChannelLayout finalLayout,
                                   const LoadHandler& completion) {
    std::unique_ptr<juce::AudioFormatReader> reader = OPJuceHelper::createReaderFor(filepath);
    if(!reader) {
        completion(nullptr, nullptr);
        return;
    }

    auto buffer = std::make_unique<OPSampleBuffer>(reader->lengthInSamples, reader->numChannels, OPChannelLayout::SEPARATED);

    reader->read(buffer->getSeparatedChannelPointerArray(), buffer->getChannelCount(), 0, reader->lengthInSamples);

    buffer->setChannelLayout(finalLayout);

    completion(std::move(buffer), std::move(reader));
}
