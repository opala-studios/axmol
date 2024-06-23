//
// Created by Ricardo on 5/31/2021.
//

#ifndef OPAUDIOIOCALLBACK_H
#define OPAUDIOIOCALLBACK_H

#include <cstdint>
#include <functional>

class OPBaseAudioIOCallback {
public:
    typedef std::function<void(const float** inputChannelData,
                               int totalNumInputChannels,
                               float** outputChannelData,
                               int totalNumOutputChannels,
                               int numSamples)> IOCallback;
public:

    OPBaseAudioIOCallback(size_t priority) : _priority(priority) {}

    virtual void audioDeviceIOCallbackInt(
        const float** inputChannelData,
        int totalNumInputChannels,
        float** outputChannelData,
        int totalNumOutputChannels,
        int numSamples) = 0;

    size_t getPriority() { return _priority; }

protected:
    size_t _priority;

};


class OPAudioIOCallback : public OPBaseAudioIOCallback {
public:
    OPAudioIOCallback(IOCallback callback, size_t priority = 128) : OPBaseAudioIOCallback(priority), _callback(std::move(callback)) {}

    void audioDeviceIOCallbackInt(
            const float** inputChannelData,
            int totalNumInputChannels,
            float** outputChannelData,
            int totalNumOutputChannels,
            int numSamples) override {
        _callback(inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
    }

private:
    IOCallback _callback;
};


#endif //OPAUDIOIOCALLBACK_H
