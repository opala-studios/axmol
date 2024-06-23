//
//  OPAudioTransform.cpp
//  opalib-ios
//
//  Created by Adilson Tavares on 15/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAudioTransform.hpp"

#include <JuceHeader.h>

#include <algorithm>
#include <climits>

#include "../Debug/OPDebug.hpp"
#include "./OPAudioWriter.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"

USING_NS_CC;
using namespace juce;

OPAudioTransform* OPAudioTransform::create(const std::string& inputPath, const std::string& outputPath)
{
    auto transform = new (std::nothrow) OPAudioTransform();
    if (transform && transform->init(inputPath, outputPath)) {
        transform->autorelease();
        return transform;
    }

    delete transform;
    transform = nullptr;
    return nullptr;
}

bool OPAudioTransform::init(const std::string& inputPath, const std::string& outputPath)
{
    _inputPath = inputPath;
    _outputPath = outputPath;

    _volume = 1;
    _pitch = 1;

    return true;
}

bool OPAudioTransform::apply(OPWaveformRange* range, OPAudioFormat::FORMAT format)
{
    auto inputFile = File(_inputPath);

    AudioTransportSource transportSource;
    AudioFormatManager formatManager;

    formatManager.registerBasicFormats();

    const int bufferSize = 1024;

    FileInputStream input (inputFile);
    if (!input.openedOk()) {
        return false;
    }

    std::unique_ptr<BufferedInputStream> bufferedInputStream(new BufferedInputStream(input, bufferSize));

    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(std::move(bufferedInputStream)));
    std::unique_ptr<AudioFormatReaderSource> source(new AudioFormatReaderSource(reader.get(), false));

    transportSource.setGain(_volume);
    transportSource.setSource(source.get());
    transportSource.start();


    std::unique_ptr<ResamplingAudioSource> resampler(new ResamplingAudioSource(&transportSource, false));
    resampler->setResamplingRatio(_pitch);

    auto outputFile = File(_outputPath);
    outputFile.deleteFile();

    std::unique_ptr<AudioFormatWriter> writer(
            OPAudioWriter::getWriter(outputFile.createOutputStream().release(),
                                     format,
                                     reader->sampleRate,
                                     reader->numChannels,
                                     reader->bitsPerSample));

    resampler->prepareToPlay(bufferSize, reader->sampleRate);

    int64 startPosition = 0;
    int64 endPosition = 0;

    if (range) {
        startPosition = (int64)range->getStartInSamples();
        endPosition = (int64)range->getEndInSamples();
    } else {
        startPosition = 0;
        endPosition = source->getTotalLength();
    }

    transportSource.setNextReadPosition(startPosition);
    transportSource.prepareToPlay(bufferSize, reader->sampleRate);

    writer->writeFromAudioSource(*resampler, endPosition - startPosition, bufferSize);

    transportSource.setSource(nullptr);

    return true;
}

OPAudioTransform::~OPAudioTransform()
{
    OPDebug::log("OPAudioTransform", "destroying audio transform");
}
