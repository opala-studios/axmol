//
//  OPAudioTransform.hpp
//  opalib-ios
//
//  Created by Adilson Tavares on 15/07/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPAudioTransform_hpp
#define OPAudioTransform_hpp

#include <cocos2d.h>
#include "./OPWaveformRange.hpp"
#include "./OPAudioBuffer.hpp"
#include "./OPAudioFormat.hpp"

class OPAudioTransform : public cocos2d::Ref
{
public:
    typedef std::function<void()> SL_ApplyFinishedCallback;
    
    static OPAudioTransform* create(const std::string& inputPath, const std::string& outputPath);
    virtual bool init(const std::string& inputPath, const std::string& outputPath);
    
    ~OPAudioTransform();
    
    CC_SYNTHESIZE(float, _pitch, Pitch);
    CC_SYNTHESIZE(float, _volume, Volume);

    OPWaveformRange* _range;
    bool apply(OPWaveformRange* range, OPAudioFormat::FORMAT format);
    
protected:
    std::string _inputPath;
    std::string _outputPath;
};

#endif /* OPAudioTransform_hpp */
