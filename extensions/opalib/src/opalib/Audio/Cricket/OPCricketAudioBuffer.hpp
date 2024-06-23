//
// Created by Ricardo on 04/10/21.
//

#ifndef OPSAMPLEBUFFER_HPP
#define OPSAMPLEBUFFER_HPP

#include <string>
#include <functional>
#include <ck/audio/sample.h>

class OPCricketAudioBuffer : public std::enable_shared_from_this<OPCricketAudioBuffer> {
public:
    typedef std::function<void(bool)> LoadCallback;
public:

    static std::shared_ptr<OPCricketAudioBuffer> create(const std::string& filepath);

    void loadAsync(LoadCallback callback);

    int getSampleCount() const;
    int getChannelCount() const;
    const float* getData() const;
    float* getData();
    const Cki::Sample& getSample() const;
    const Cki::SampleInfo& getSampleInfo() const;

private:
    OPCricketAudioBuffer(std::string filepath);

    void dispatchLoadCallback(Cki::Sample* sample);
    void destroySample(Cki::Sample* sample);

private:
    std::string _filepath;
    Cki::Sample* _sample = nullptr;
    std::function<void(bool)> _loadCallback;
};


#endif //OPSAMPLEBUFFER_HPP
