//
// Created by Ricardo on 15/10/21.
//

#ifndef OPJUCESTREAMEDPLAYERIMPLEMENTATION_HPP
#define OPJUCESTREAMEDPLAYERIMPLEMENTATION_HPP

#include "OPJucePlayerImplementation.hpp"


class OPJuceStreamedPlayerImplementation : public OPJucePlayerImplementation {
public:
    OPJuceStreamedPlayerImplementation(OPJuceEngine* engine, const std::string& filePath);
    ~OPJuceStreamedPlayerImplementation();

    double getSourceSampleRate() override;

protected:
    bool isPlayerReady() const override;

    std::unique_ptr<juce::PositionableAudioSource> createSource() override;
private:
    std::string _filePath = "";
    std::unique_ptr<juce::File> _file { nullptr };
    std::unique_ptr<juce::AudioFormatReader> _reader { nullptr };
    juce::TimeSliceThread _backgroundThread{"OPJuceStreamedPlayerImplementation"};
};


#endif //OPJUCESTREAMEDPLAYERIMPLEMENTATION_HPP
