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

public:
    typedef std::function<void(float frequence, float probability)> SL_PitchStimationCallback;
    
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
    
    SL_PitchStimationCallback _pitchStimationCallback;
    int16_t _desiredWindowSize = 0;
     
public:
    void setPitchStimationCallback(SL_PitchStimationCallback pitchStimationCallback);
    
    OPInputPitchEstimator(OPAudioEngine* manager);
    ~OPInputPitchEstimator();

};
#endif /* OPInputAnalyzer_hpp */
