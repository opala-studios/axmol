//
// Created by Ricardo on 30/09/21.
//

#ifndef OPCRICKETAUDIOSTREAM_HPP
#define OPCRICKETAUDIOSTREAM_HPP

#include <ck/customstream.h>
#include <string>
#include <functional>
#include <opalib/Audio/OPAudioBuffer.hpp>

#include <JuceHeader.h>

class OPCricketAudioStream : public CkCustomStream {
public:
    OPCricketAudioStream(std::string filepath);

private:
    ~OPCricketAudioStream() override;

    void init() override;

    bool isFailed() const override;

    int getChannels() const override;

    int getSampleRate() const override;

    int getLength() const override;

    int read(short *buf, int frames) override;

    void setPlayPosition(int frame) override;

    int getPlayPosition() const override;
private:
    std::string _filepath;
    std::unique_ptr<juce::AudioFormatReader> _reader;
    int _position = 0;
    bool _inited = false;
};


#endif //OPCRICKETAUDIOSTREAM_HPP
