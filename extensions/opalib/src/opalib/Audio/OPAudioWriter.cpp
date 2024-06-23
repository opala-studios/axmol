//
//  OPAudioWriter.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 03/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPAudioWriter.hpp"
#include "opalib/Audio/Juce/OPJuceEngine.hpp"

USING_NS_CC;

using namespace juce;

bool OPAudioWriter::init(){
    return true;
}

void OPAudioWriter::write(OPMemoryPlayer* player, OPWaveformRange* selection, const std::string& destFilePath, OPAudioFormat::FORMAT audioFormat)
{
    
    if(!player || !selection || destFilePath.compare("") == 0)
    {
        CCASSERT(false,"Something is missing! Please check the inputs.");
    }
    
    AudioBuffer<float> buffer;
    
    int rangeSize = selection->getEndInSamples() - selection->getStartInSamples();

    buffer.setSize(player->getAudioBuffer()->getChannelCount(), rangeSize);
    
    for(int c = 0; c < player->getAudioBuffer()->getChannelCount(); c++)
    {
        for(int i = 0; i < rangeSize; i++)
        {
            int idx = selection->getStartInSamples() + i;
            int loopedIdx = idx >= player->getAudioBuffer()->getSampleCount() ? player->getAudioBuffer()->getSampleCount() - idx : idx;
            buffer.setSample(c, i, player->getAudioBuffer()->getSampleBuffer()->getSample(c, loopedIdx));
        }
    }
    
    write(buffer,destFilePath,audioFormat);
}

void OPAudioWriter::write(OPAudioBuffer* audioBuffer, OPWaveformRange* selection, const std::string& destFilePath, OPAudioFormat::FORMAT audioFormat)
{
    
    if(!audioBuffer || !selection || destFilePath.compare("") == 0)
    {
        CCASSERT(false,"Something is missing! Please check the inputs.");
    }
    
    AudioBuffer<float> buffer;
    
    int rangeSize = selection->getEndInSamples() - selection->getStartInSamples();

    buffer.setSize(audioBuffer->getChannelCount(), rangeSize);
    
    for(int c = 0; c < audioBuffer->getChannelCount(); c++)
    {
        for(int i = 0; i < rangeSize; i++)
        {
            int idx = selection->getStartInSamples() + i;
            int loopedIdx = idx >= audioBuffer->getSampleCount() ? audioBuffer->getSampleCount() - idx : idx;
            buffer.setSample(c, i, audioBuffer->getSampleBuffer()->getSample(c, loopedIdx));
        }
    }
    
    write(buffer,destFilePath,audioFormat);
}

void OPAudioWriter::write(const AudioBuffer<float>& buffer,const std::string& destFilePath, OPAudioFormat::FORMAT audioFormat)
{
    File file(destFilePath);
    file.deleteFile();

    AudioFormat* format;

    switch (audioFormat) {
        case OPAudioFormat::FORMAT::WAV:
            format = new WavAudioFormat();
            break;
        case OPAudioFormat::FORMAT::OGG:
            format = new OggVorbisAudioFormat();
            break;
        case OPAudioFormat::FORMAT::FLAC:
            format = new FlacAudioFormat();
            break;
        case OPAudioFormat::FORMAT::MP3:
            CCASSERT(false,"Format not supported for writer.");
        default:
            break;
    }

    std::unique_ptr<AudioFormatWriter> writer;

    writer.reset(format->createWriterFor(
                    new FileOutputStream(file),
                    44100,
                    buffer.getNumChannels(),
                    16,
                    {},
                    0));

    if (writer != nullptr)
     writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    delete format;
}

AudioFormatWriter* OPAudioWriter::getWriter(OutputStream* fileStream, OPAudioFormat::FORMAT format, double sampleRate, int numberOfChannels, int bitrate)
{
   std::unique_ptr<AudioFormat> writerFormat;

    switch (format)
    {
       case OPAudioFormat::FORMAT::WAV:
            writerFormat.reset( new WavAudioFormat());
           break;
       case OPAudioFormat::FORMAT::OGG:
           writerFormat.reset(new OggVorbisAudioFormat());
           break;
      case OPAudioFormat::FORMAT::FLAC:
           writerFormat.reset( new FlacAudioFormat());
           break;
       case OPAudioFormat::FORMAT::MP3:
           CCASSERT(false,"Format not supported for writer.");
           break;
           default:
           break;
    }

    std::unique_ptr<AudioFormatWriter> writer(writerFormat->createWriterFor (fileStream, sampleRate, numberOfChannels, bitrate, {}, 0));
    if (writer)
    {
        return writer.release();
    }
    
    return nullptr;
}


OPAudioWriter::~OPAudioWriter() {}


