//
//  OPAudioBuffer.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 17/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAudioBuffer_hpp
#define OPAudioBuffer_hpp

#include <JuceHeader.h>
#include <cstdlib>

typedef std::function<void(bool)> LoadAsyncHandler;

class OPAudioBuffer: public std::enable_shared_from_this<OPAudioBuffer>
{
public:
    enum class ChannelLayout {
        INTERLEAVED = 0,
        SEPARATED = 1
    };

    /**
     * Helper class so we can easily manipulate multiple channel layouts
     */

    class SampleBuffer {
    public:
        SampleBuffer(int sampleCount, int channelCount, ChannelLayout layout) :
                _sampleCount(sampleCount), _sampleCapacity(sampleCount), _channelCount(channelCount), _channelLayout(layout){
            allocateChannelData();
        }

        SampleBuffer(ChannelLayout layout) : SampleBuffer(0, 0, layout) {}

        SampleBuffer(SampleBuffer&& other) {
            move(other);
        }

        ~SampleBuffer(){
            deallocateChannelData();
        }

        SampleBuffer& operator=(SampleBuffer&& other){
            move(other);
            return *this;
        }

        void move(SampleBuffer& other){
            deallocateChannelData();
            _sampleCount = other._sampleCount;
            _channelCount = other._channelCount;
            _channelLayout = other._channelLayout;
            _data[0] = other._data[0];
            _data[1] = other._data[1];
            other._data[0] = nullptr;
            other._data[1] = nullptr;
            other._sampleCount = 0;
            other._channelCount = 0;
        }

        void copyFrom(int dstChannel, int dstStartSample, const float* srcData, int sampleCount){
            assert(dstStartSample + sampleCount <= _sampleCount);
            assert(isAllocated());
            switch(_channelLayout) {
                case ChannelLayout::INTERLEAVED: {
                    assert(_data[0] != nullptr);
                    /**
                     * We need to make sure that we only write to the specified dst channel
                     * in our interleaved buffer, so if our dstChannel is 0, we will only
                     * write to samples which are even (0, 2, 4, 6...). And if dst channel is
                     * 1, we will write to odd indices (1, 3, 5, 7...).
                     **/
                    int writeSample = dstStartSample + dstChannel;
                    for (int sample = 0; sample < sampleCount; sample++) {
                        _data[0][writeSample] = srcData[sample];

                        //will skip to the next sample of this channel
                        writeSample += _channelCount;
                    }
                } break;
                case ChannelLayout::SEPARATED: {
                    assert(_data[dstChannel] != nullptr);
                    std::memcpy(_data[dstChannel], srcData, sizeof(float) * sampleCount);
                } break;
            }
        }

        void copyTo(int srcChannel, int srcStartSample, float* dstData, int sampleCount){
            assert(srcStartSample + sampleCount <= _sampleCount);
            assert(isAllocated());
            switch(_channelLayout){
                case ChannelLayout::INTERLEAVED:{
                    assert(_data[0] != nullptr);
                    /**
                     * We need to make sure that we only read from the specified src channel
                     * in our interleaved buffer, so if our srcChannel is 0, we will only
                     * read from samples which are even (0, 2, 4, 6...). And if src channel is
                     * 1, we will read from odd indices (1, 3, 5, 7...).
                     **/
                    int readSample = srcStartSample + srcChannel;
                    for (int sample = 0; sample < sampleCount; sample++) {
                        dstData[sample] = _data[srcChannel][readSample];

                        //will skip to the next sample of this channel
                        readSample += _channelCount;
                    }
                } break;
                case ChannelLayout::SEPARATED:{
                    assert(_data[srcChannel] != nullptr);
                    std::memcpy(dstData, _data[srcChannel] + srcStartSample, sizeof(float) * sampleCount);
                } break;
            }
        }

        void copyToInterleaved(int srcStartSample, float* dst, int sampleCount) const {
            assert(srcStartSample + sampleCount <= _sampleCount);
            assert(isAllocated());
            switch(_channelLayout){
                case ChannelLayout::INTERLEAVED:
                    std::memcpy(dst, _data[0] + (srcStartSample * _channelCount), sizeof(float) * sampleCount * _channelCount);
                    break;
                case ChannelLayout::SEPARATED:
                    
                    for (int sample = 0; sample < sampleCount; sample++){
                        int readSampleIndex = sample + srcStartSample;//separated index
                        int writeSampleIndex = sample * _channelCount;//interleaved index
                        
                        dst[writeSampleIndex] = _data[0][readSampleIndex];
                        dst[writeSampleIndex + 1] = _data[1][readSampleIndex];
                    }
                    
                    break;
            }
        }
        
        void copyFromInterleaved(int dstStartSample, const float* src, int sampleCount){
            assert(dstStartSample + sampleCount <= _sampleCount);
            assert(isAllocated());
            switch(_channelLayout){
                case ChannelLayout::INTERLEAVED:
                    std::memcpy(_data[0] + dstStartSample * _channelCount, src, sizeof(float) * sampleCount * _channelCount);
                    break;
                case ChannelLayout::SEPARATED:
                    
                    for (int sample = 0; sample < sampleCount; sample++){
                        int readSampleIndex = sample * _channelCount;//interleaved index
                        int writeSampleIndex = sample + dstStartSample;//separated index
                        
                        _data[0][writeSampleIndex] = src[readSampleIndex];
                        _data[1][writeSampleIndex] = src[readSampleIndex + 1];
                    }
                    
                    break;
            }
        }

        float* getSeparatedChannelPointer(int channel, int startSample = 0){
            assert(_channelLayout == ChannelLayout::SEPARATED);
            assert(isAllocated());
            return _data[channel] + startSample;
        }

        float** getSeparatedChannelPointerArray(){
            assert(_channelLayout == ChannelLayout::SEPARATED);
            assert(isAllocated());
            return _data;
        }

        float* getInterleavedChannelPointer(int startSample = 0){
            assert(_channelLayout == ChannelLayout::INTERLEAVED);
            assert(isAllocated());
            return _data[0] + (startSample * _channelCount);
        }


        /**
         * Changes the buffer channel layout to the one specified
         * NOTE: this will cause some memory allocations and deallocations, as well as lots of copies
         * It may be REALLY expensive, so be careful
         **/
        void setChannelLayout(ChannelLayout newLayout){
            if (newLayout == _channelLayout){
                return;
            }

            if (isAllocated()) {
                switch (newLayout) {
                    case ChannelLayout::INTERLEAVED: {
                        //separated => interleaved
                        float *interleavedChannels = new float[_sampleCount * _channelCount];
                        juce::AudioDataConverters::interleaveSamples(
                                const_cast<const float **>(_data),
                                interleavedChannels,
                                _sampleCount,
                                _channelCount
                        );
                        deallocateChannelData();
                        _data[0] = interleavedChannels;
                    } break;
                    case ChannelLayout::SEPARATED: {
                        //interleaved => separated
                        float *separatedChannels[2]{
                                0 < _channelCount ? new float[_sampleCount] : nullptr,
                                1 < _channelCount ? new float[_sampleCount] : nullptr
                        };
                        juce::AudioDataConverters::deinterleaveSamples(
                                _data[0],
                                separatedChannels,
                                _sampleCount,
                                _channelCount
                        );
                        deallocateChannelData();
                        _data[0] = separatedChannels[0];
                        _data[1] = separatedChannels[1];
                    } break;
                }
                _allocated = true;
            }
            _channelLayout = newLayout;
        }

        float getSample(int channelIndex, int sampleIndex){
            switch(_channelLayout){
                case ChannelLayout::INTERLEAVED:
                    return _data[0][sampleIndex * _channelCount] + channelIndex;
                case ChannelLayout::SEPARATED:
                    return _data[channelIndex][sampleIndex];
            }
        }
        
        void setSize(int newSize){
            assert(newSize <= _sampleCapacity && "We currently cannot expand our sample buffer.");
            _sampleCount = newSize;
        }

        int getSampleCount() const {
            return _sampleCount;
        }

        int getChannelCount() const {
            return _channelCount;
        }

        ChannelLayout getChannelLayout() const {
            return _channelLayout;
        }

        bool isAllocated() const {
            return _allocated;
        }

        void clear(){
            deallocateChannelData();
        }

    private:

        void allocateChannelData(){
            if (_channelCount == 0 || _sampleCount == 0 || _sampleCapacity == 0){
                return;
            }
            switch(_channelLayout){
                case ChannelLayout::INTERLEAVED:
                    _data[0] = new float[_sampleCount * _channelCount];
                    break;
                case ChannelLayout::SEPARATED:
                    for (int c = 0; c < _channelCount; c++){
                        _data[c] = new float[_sampleCount];
                    }
                    break;
            }
            _allocated = true;
        }

        void deallocateChannelData(){
            delete[] _data[0];
            delete[] _data[1];
            _data[0] = nullptr;
            _data[1] = nullptr;
            _allocated = false;
        }

    private:
        int _sampleCount = 0;

        int _channelCount = 0;
        
        int _sampleCapacity = 0;

        /**
        * The current channel layout of this audio buffer
        *
        * In a simple example, let's consider a stereo audio buffer, with channels named A and B:
        * When our channel layout is ChannelLayout::SEPARATED, our samples will be contained
        * in two distinct float arrays, and in memory will be like AAAA... and BBBB...
        *
        * And when our channel layout is ChannelLayout::INTERLEAVED, our samples will be contained
        * in a single float array, alternately like ABABABABAB...
        *
        * Audio engines and/or audio processing techniques may use different channel layouts,
        * so it's important to know which one you should use.
        */
        ChannelLayout _channelLayout;

        float* _data[2]{nullptr, nullptr};

        bool _allocated = false;
    };

public:
    /** Creates an instance os OPAudioBuffer with a shared smart pointer. The shared smart pointer is needed
        to manage the load lifecicle of the objcet in the OPAudioLoader class in this file. Thats is the only way using this class as the available constructors are private.

        @param filePath         a std::string with the full path to a valid audio file.
          
        @returns                a shared_ptr to the OPAudioBuffer object that only can be used this way.
    */
    static std::shared_ptr<OPAudioBuffer> create(const std::string& filePath, ChannelLayout channelLayout);
    
    /** Destructor. */
    ~OPAudioBuffer();


    //==============================================================================
    /** Returns the file path of file used to load the buffer.
    */
    const std::string& getFilePath() const;

    //==============================================================================
    /** Returns the sample rate of the original file loaded.
    */
    double getSampleRate();

    /** Returns the current sample count of this buffer
    */
    int getSampleCount() const;

    /** Returns the current channel count of this buffer
    */
    int getChannelCount() const;

    /** Returns the current sample buffer*/
    SampleBuffer* getSampleBuffer();

    //==============================================================================
    /** Execute the load process of the file described in the create function.
        If no file is available the completion will be called with false.
     
        Note that if this object was destroyed before the load process finish, the buffer will be destroyed and the completion will never be triggered.

        @param completion         a function handler std::function<void(bool)> to receive a boolean indicating                             the process's success.
    */
    void loadAsync(const LoadAsyncHandler& completion);

    //==============================================================================
    /** Execute the load process of the file described in the create function on the same thread that this function is called.
     *
        Return a boolean indicating the process's success
    */
    bool loadSync();
    
    //==============================================================================
    /** For now it just set the loaded flag to false.
     @todo  Implement the memory unload.
    */
    void unload();
    
    //==============================================================================
    /** Returns true if the buffer was loaded successfully, false otherwise.
    */
    bool isLoaded();
    
    //==============================================================================
    /** Returns true if the buffer is in the middle of loading process, false otherwise.
    */
    bool isLoading();
    
    /** Returns the lenght of the loaded buffer in seconds.
     */
    double getLenghtInSeconds();

protected:

    std::atomic_bool _loading{false};

    std::unique_ptr<SampleBuffer> _buffer { nullptr };

    bool _success = false;
    
private:
    /** Private Constructor.
     */
    OPAudioBuffer(std::string  filePath, ChannelLayout channelLayout);
    
    /** Inaccessible default constructor.
     */
    OPAudioBuffer() = delete;
    
    std::string _filePath;
    
    std::unique_ptr<juce::AudioFormatReader> _reader { nullptr };
};

typedef OPAudioBuffer::SampleBuffer OPSampleBuffer;
typedef OPAudioBuffer::ChannelLayout OPChannelLayout;

class OPAudioLoader
{
public:
    typedef std::function<void(std::unique_ptr<OPSampleBuffer> buffer, std::unique_ptr<juce::AudioFormatReader> reader)> LoadHandler;
    static void loadBufferAsync(const std::string& filePath, OPChannelLayout finalLayout, const std::weak_ptr<OPAudioBuffer>& owner, const LoadHandler& completion);
    static void loadBufferSync(const std::string& filepath, OPChannelLayout finalLayout, const LoadHandler& completion);
};


//Helper class for objects that will hold an audio buffer
class OPAudioBufferHolder {
public:
    virtual std::shared_ptr<OPAudioBuffer> getAudioBuffer() = 0;
};
#endif /* OPAudioBuffer_hpp */
