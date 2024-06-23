//
// Created by Ricardo on 20/10/21.
//

#ifndef OPCRICKETBUFFERSOURCE_HPP
#define OPCRICKETBUFFERSOURCE_HPP


#include <ck/audio/audiosource.h>
#include <opalib/Audio/OPAudioBuffer.hpp>

// Should this REALLY be here?
#include <juce_audio_basics/utilities/juce_ADSR.h>

class OPCricketBufferSource : public Cki::AudioSource {
public:
    OPCricketBufferSource();
    OPCricketBufferSource(std::shared_ptr<OPAudioBuffer> buffer);
    ~OPCricketBufferSource() override;

    void setAudioBuffer(std::shared_ptr<OPAudioBuffer> buffer);
    
    void prepareToPlay();
    
    void prepareToStop();

    int read(void *buf, int blocks) override;

    int getNumBlocks() const override;

    void setBlockPos(int block) override;

    int getBlockPos() const override;

    void reset() override;

    const Cki::SampleInfo &getSampleInfo() const override;

    bool isInited() const override;

    bool isReady() const override;

    bool isFailed() const override;

    bool isDone() const override;

    void setLoop(int startFrame, int endFrame) override;

    void getLoop(int &startFrame, int &endFrame) const override;

    void setLoopCount(int i) override;

    int getLoopCount() const override;

    int getCurrentLoop() const override;

    void releaseLoop() override;

    bool isLoopReleased() const override;

    bool isLooping() const;
private:
    
    void applyEnvelopeToBuffer(float* buffer, int channelCount, int sampleCount);

private:
    std::shared_ptr<OPAudioBuffer> _buffer;
    Cki::SampleInfo _sampleInfo;
    juce::ADSR _adsr;
    int _position = 0;
    int _loopCurrent = 0;
    bool _releaseLoop = false;
};


#endif //OPCRICKETBUFFERSOURCE_HPP
