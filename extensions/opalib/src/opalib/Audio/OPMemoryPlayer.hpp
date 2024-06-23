//
//  OPMemoryPlayer.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 09/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPMemoryPlayer_hpp
#define OPMemoryPlayer_hpp

#include "OPPlayer.hpp"
#include "OPWaveformRange.hpp"
#include <opalib/Utils/OPAsyncTask.hpp>
#include <soundtouch/include/SoundTouch.h>

class OPMemoryPlayer: public OPPlayer
{
public:
	typedef std::function<void(int bufferIndex)> ChangeTempoBufferProcessedHandler;
public:
    static std::unique_ptr<OPMemoryPlayer> createUnique(const std::string& filePath,const LoadAsyncHandler& loadedCallback);
    static std::shared_ptr<OPMemoryPlayer> createShared(const std::string& filePath,const LoadAsyncHandler& loadedCallback);

    OPMemoryPlayer(const std::string& filePath, const LoadAsyncHandler& loadedCallback);
    ~OPMemoryPlayer();

protected:
	void createImplementation() override;

	void destroyImplementation() override;

	void createImplementation(const std::string& filePath, const LoadAsyncHandler& loadedCallback);

public:

	std::shared_ptr<OPAudioBuffer> getAudioBuffer();
    void playRange(OPWaveformRange* range);
    virtual void stop() override;

    void changeTempoAsync(float tempo, int bufferCount, const ChangeTempoBufferProcessedHandler& onBufferProcessed);

	double getRangeDuration(OPWaveformRange* range);

    void update(float dt) override;
    
private:
    void clearRangeIfNeeded();
    void setCurrentEndPositionInSamples();
    size_t changeTempo(const float *inputChannels, float *outputChannels,
					   size_t channelCount, size_t inputSampleCount,
					   soundtouch::SoundTouch &soundTouch, bool isLastBuffer);
    int _forcedEndPosition = -1;
    std::string _filepath;
	LoadAsyncHandler _loadedCallback;
    
    bool _isPlayingRange = false;
    OPWaveformRange* _currentRange { nullptr };
	OPCPUAsyncTask _changeTempoTask;
    double _currentTempo = 1.0f;
    bool _isReady = false;
};


#endif /* OPMemoryPlayer_hpp */
