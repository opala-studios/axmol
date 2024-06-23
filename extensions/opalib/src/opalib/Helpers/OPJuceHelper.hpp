//
//  OPJuceHelper.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 14/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPJuceHelper_hpp
#define OPJuceHelper_hpp

#include <JuceHeader.h>

class OPJuceHelper
{
public:
    static std::unique_ptr<juce::AudioFormatReader> createReaderFor(const std::string& filePath);
};

#endif /* OPJuceHelper_hpp */
