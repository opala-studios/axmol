//
// Created by Ricardo on 05/10/21.
//

#ifndef OPCRICKETSYNTHESIZER_HPP
#define OPCRICKETSYNTHESIZER_HPP

#include <string>
#include <unordered_map>
#include <set>
#include "OPCricketBufferPlayer.hpp"
#include "OPCricketAudioBuffer.hpp"
#include <opalib/Audio/OPSynthesizerImplementation.hpp>


class OPCricketSynthesizerImplementation : public OPSynthesizerImplementation {
private:
    struct Voice {
        std::shared_ptr<OPCricketBufferPlayer> player;
        OPSynthItem* item = nullptr;
    };
    
    struct Sampler {
        std::shared_ptr<OPAudioBuffer> audioBuffer;
        float speed = 1.0f;
        float volume = 1.0f;
        bool looping = false;
    };
    
public:
    OPCricketSynthesizerImplementation(int numberOfVoices);
    ~OPCricketSynthesizerImplementation();

    void play(OPSynthItem *item) override;
    void stop(OPSynthItem *item) override;
    void loadItem(OPSynthItem *item, const LoadItemHandler &handler) override;
    void unloadItem(OPSynthItem *item) override;
    void unloadAll() override;

    bool isLoaded(OPSynthItem* item) const override;

    void setIsLooping(OPSynthItem* item, bool looping) override;
    bool getIsLooping(OPSynthItem* item) const override;
    
    void setSpeed(float speed) override;
    void setSpeed(OPSynthItem* item, float speed) override;
    float getSpeed(OPSynthItem* item) const override;
    
    void setVolume(float volume) override;
    void setVolume(OPSynthItem* item, float volume) override;
    float getVolume(OPSynthItem* item) const override;

private:

    bool findAvailableVoice(size_t& availableVoiceIndex);
    bool findVoicePlayingItem(OPSynthItem* item, size_t& voiceIndex);
    void updateVoice(OPSynthItem* item);
    void updateAllVoices();

private:
    std::unordered_map<OPSynthItem*, Sampler> _itemToSampler;
    std::vector<Voice> _voices;

};




#endif //OPCRICKETSYNTHESIZER_HPP
