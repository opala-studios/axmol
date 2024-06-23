//
// Created by Ricardo on 15/10/21.
//

#include <opalib/Helpers/OPJuceHelper.hpp>
#include "OPJuceStreamedPlayerImplementation.hpp"
#include "OPStreamedAudioSource.hpp"
#include <cocos2d.h>


bool OPJuceStreamedPlayerImplementation::isPlayerReady() const {
    return true;
}

std::unique_ptr<juce::PositionableAudioSource> OPJuceStreamedPlayerImplementation::createSource() {
    _backgroundThread.startThread();
    _reader = OPJuceHelper::createReaderFor(_filePath);
    auto formatReaderSource = std::make_unique<juce::AudioFormatReaderSource>(_reader.get(), false);
    auto source = std::make_unique<juce::BufferingAudioSource>(formatReaderSource.release(), _backgroundThread, true, 4096 * 8, _reader->numChannels);
    return std::move(source);
}

OPJuceStreamedPlayerImplementation::OPJuceStreamedPlayerImplementation(
        OPJuceEngine* engine,
        const std::string &filePath) : OPJucePlayerImplementation(engine) {

    if(!cocos2d::FileUtils::getInstance()->isFileExist(filePath)){
        jassertfalse;
    }

    _filePath = filePath;
    setup();
}

OPJuceStreamedPlayerImplementation::~OPJuceStreamedPlayerImplementation() {
    // If we don't unhook here, our reader may be used in
    // our audio callback during this object destruction, causing an explosion
    unhook();
    _backgroundThread.stopThread(1000);
}

double OPJuceStreamedPlayerImplementation::getSourceSampleRate() {
    if(_reader)
        return _reader->sampleRate;
    else
        return 44100.0;
}
