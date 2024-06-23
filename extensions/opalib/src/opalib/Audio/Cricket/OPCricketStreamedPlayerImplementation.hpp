//
// Created by Ricardo on 20/10/21.
//

#ifndef OPCRICKETSTREAMEDPLAYERIMPLEMENTATION_HPP
#define OPCRICKETSTREAMEDPLAYERIMPLEMENTATION_HPP

#include <opalib/Audio/OPPlayerImplementation.hpp>
#include <ck/sound.h>
#include <string>


class OPCricketStreamedPlayerImplementation : public OPPlayerImplementation {
public:
    OPCricketStreamedPlayerImplementation(const std::string& filepath);

    ~OPCricketStreamedPlayerImplementation() override;

    void play() override;

    void stop() override;
    
    void pause() override;

    void resume() override;

    int getLengthInSamples() const override;

    double getLengthInSeconds() const override;

    bool isPlaying() const override;
    
    bool isReady() const override;

    void setSpeed(double speed) override;

    double getSpeed() const override;

    void setLooping(bool shouldLoop) override;

    bool isLooping() const override;

    void setVolume(float d) override;

    float getVolume() const override;

    void setCurrentPositionInSamples(int i) override;

    int getCurrentPositionInSamples() const override;

    double getSourceSampleRate() override;

private:
    CkSound* _sound = nullptr;
    int _lastPausePosition = 0;
    bool _paused = false;
};


#endif //OPCRICKETSTREAMEDPLAYERIMPLEMENTATION_HPP
