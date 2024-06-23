//
// Created by Ricardo on 07/10/21.
//

#ifndef OPAUDIOENGINE_HPP
#define OPAUDIOENGINE_HPP

#include <opalib/Audio/OPBaseAudioIOCallback.h>
#include <opalib/Audio/OPAudioEngineImplementation.hpp>
#include <opalib/Audio/OPSynthesizerImplementation.hpp>
#include <eventbus/EventBus.h>
#include <vector>
#include <mutex>
#include "OPPlayerImplementation.hpp"
#include "OPAudioBuffer.hpp"

class OPAudioEngine {
public:
    struct EventAudioPause {};
    struct EventAudioResume {};
    struct EventDestroyImplementation {};
    struct EventCreateImplementation {};
    typedef std::function<void()> ImplementationReadyCallback;
    struct AudioConfig {
        std::string engineType;
    };
public:
    class AudioIOCallbackArray {
    public:
        void addAudioIOCallback(OPBaseAudioIOCallback* callback);
        void removeAudioIOCallback(OPBaseAudioIOCallback* callback);
        void sort();

        std::vector<OPBaseAudioIOCallback*>::iterator begin() { return _ioCallbacks.begin(); }
        std::vector<OPBaseAudioIOCallback*>::iterator end() { return _ioCallbacks.end(); }

        std::vector<OPBaseAudioIOCallback*>::const_iterator begin() const { return _ioCallbacks.cbegin(); }
        std::vector<OPBaseAudioIOCallback*>::const_iterator end() const { return _ioCallbacks.cend(); }

    private:
        std::vector<OPBaseAudioIOCallback*> _ioCallbacks;
    };
public:
    OPAudioEngine();
    virtual ~OPAudioEngine();

    void addAudioIOCallback(OPBaseAudioIOCallback* callback);
    void removeAudioIOCallback(OPBaseAudioIOCallback* callback);

    template<typename T>
    void changeImplementation(bool autoInitialize = true){
        changeImplementation(new T(), autoInitialize);
    }
    
    bool isImplementationReady() const;
    void setImplementationReadyCallback(ImplementationReadyCallback callback);

    void pause();
    void resume();
    void shutDown();
    void initialize();

    void setInputChannels(int channels);

    int activeInputChannels();
    int activeOutputChannels();
    double getDeviceSampleRate();
    int getAudioBufferSize();
    float getOutputLevel();
    void setUseOutputLevelMeter(bool shouldUse);

    inline bool isPaused() const { return _paused; }
    inline std::shared_ptr<Dexode::EventBus>& getEventBus() { return _eventBus; }

protected:
    
    void loadConfiguration(const std::string& filePath);

    void changeImplementation(OPAudioEngineImplementation* implementation, bool autoInitialize);

    void audioDeviceIOCallback(const float** inputChannelData,
                                  int totalNumInputChannels,
                                  float** outputChannelData,
                                  int totalNumOutputChannels,
                                  int numSamples);
    
    void incrementPendingImplementationCounter();
    void decrementPendingImplementationCounter();

    friend class OPSynthesizer;
    virtual OPSynthesizerImplementation* createSynthesizerImplementation();

    friend class OPStreamedPlayer;
    virtual OPPlayerImplementation* createStreamedPlayerImplementation(const std::string &filepath);

    friend class OPMemoryPlayer;
    virtual OPPlayerImplementation* createMemoryPlayerImplementation(const std::string& filePath, const LoadAsyncHandler &loadedCallback);

private:

    std::mutex _ioCallbackMutex;
    AudioIOCallbackArray _ioCallbackArray;
    std::shared_ptr<Dexode::EventBus> _eventBus;
    std::unique_ptr<OPAudioEngineImplementation> _implementation;
    size_t _pendingImplementationCounter = 0;
    ImplementationReadyCallback _implementationReadyCallback;
    AudioConfig _config;
    bool _ignoresDeviceMuted {true};
    bool _paused {false};

};


#endif //OPAUDIOENGINE_HPP
