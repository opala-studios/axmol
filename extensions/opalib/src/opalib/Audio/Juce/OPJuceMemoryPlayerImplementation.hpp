//
// Created by Ricardo on 18/10/21.
//

#ifndef OPJUCEMEMORYPLAYERIMPLEMENTATION_HPP
#define OPJUCEMEMORYPLAYERIMPLEMENTATION_HPP

#include <opalib/Audio/Juce/OPJucePlayerImplementation.hpp>
#include <opalib/Audio/OPAudioBuffer.hpp>

class OPJuceMemoryPlayerImplementation : public OPJucePlayerImplementation {
public:
    OPJuceMemoryPlayerImplementation(OPJuceEngine *engine,
                                     const std::string &filepath,
                                     LoadAsyncHandler callback);
    
    ~OPJuceMemoryPlayerImplementation() override;
    
    double getSourceSampleRate() override;

    std::shared_ptr<OPAudioBuffer> getAudioBuffer() override;

protected:
    bool isPlayerReady() const override;

    std::unique_ptr<juce::PositionableAudioSource> createSource() override;

private:

    LoadAsyncHandler _loadedCallback;
    std::shared_ptr<OPAudioBuffer> _audioBuffer;

};


#endif //OPJUCEMEMORYPLAYERIMPLEMENTATION_HPP
