//
//  OPInputAnalyzer.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 14/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPInputAnalyzer_hpp
#define OPInputAnalyzer_hpp

#include "./OPJuceInput.hpp"

typedef struct {
    int amplitude;
    float currentFrequency;
}AudioWindowInfo;

/**
* @struct  Yin
* @breif    Object to encapsulate the parameters for the Yin pitch detection algorithm
*/
typedef struct _Yin {
      int16_t bufferSize;            /**< Size of the audio buffer to be analysed */
      int16_t halfBufferSize;        /**< Half the buffer length */
      float* yinBuffer;        /**< Buffer that stores the results of the intermediate processing steps of the algorithm */
      float probability;        /**< Probability that the pitch found is correct as a decimal (i.e 0.85 is 85%) */
      float threshold;        /**< Allowed uncertainty in the result as a decimal (i.e 0.15 is 15%) */
  } Yin;

class OPInputPitchEstimator: public OPJuceInput {
    
protected:
    Yin _yin;
    /**
     * Initialise the Yin pitch detection object
     * @param yin        Yin pitch detection object to initialise
     * @param bufferSize Length of the audio buffer to analyse
     * @param threshold  Allowed uncertainty (e.g 0.05 will return a pitch with ~95% probability)
     */
    void Yin_init(Yin *yin, int16_t bufferSize, float threshold);

    /**
     * Runs the Yin pitch detection algortihm
     * @param  yin    Initialised Yin object
     * @param  buffer Buffer of samples to analyse
     * @return        Fundamental frequency of the signal in Hz. Returns -1 if pitch can't be found
     */
    float Yin_getPitch(Yin *yin, int16_t* buffer);

    /**
     * Certainty of the pitch found
     * @param  yin Yin object that has been run over a buffer
     * @return     Returns the certainty of the note found as a decimal (i.e 0.3 is 30%)
     */
    float Yin_getProbability(Yin *yin);
    void Yin_difference(Yin *yin, int16_t* buffer);
    void Yin_cumulativeMeanNormalizedDifference(Yin *yin);
    int16_t Yin_absoluteThreshold(Yin *yin);
    float Yin_parabolicInterpolation(Yin *yin, int16_t tauEstimate);
        

    int _numberOfFrames = 0;
    int16_t*  _inputDataInt;
    
    virtual void audioDeviceAboutToStart (AudioIODevice* device) override;
    virtual void audioDeviceStopped() override;
 
public:
    //2048 is a good window size to identify low frequences > 46.87 hz: (f = SR / WS) -> ((23.43hz) = 48000/2048)
    //Niquist theorem explains: in a window where fits 1 cycle of 23.43Hz the lowest freq we can detect is 46.87 Hz -> (2*23.43Hz)

    // 1/3 of DESIRED_WINDOW_SIZE: 512*3 = 1536 it's enough to the frequece range of a guitar > 60 Hz
    const int16_t DESIRED_BUFFER_SIZE = 480;
    
    int16_t _bufferSize = 0; // default value of juce's input buffersize
    int16_t _fixedDesiredWindowSize = 0;
    
    typedef std::function<void(float frequence, float probability)> SL_PitchStimationCallback;
    CC_SYNTHESIZE(SL_PitchStimationCallback, _pitchStimationCallback, PitchStimationCallback);
    
    bool init() override;
    CREATE_FUNC(OPInputPitchEstimator);
    ~OPInputPitchEstimator();

};
#endif /* OPInputAnalyzer_hpp */
