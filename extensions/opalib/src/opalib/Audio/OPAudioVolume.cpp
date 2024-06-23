//
//  OPAudioVolume.cpp
//  opalib-ios
//
//  Created by Opala Studios on 29/06/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#include "OPAudioVolume.hpp"


void OPAudioVolume::audioDeviceIOCallbackInt(const float **inputChannelData, int totalNumInputChannels, float **outputChannelData, int totalNumOutputChannels, int numSamples) {
	for (int channel = 0; channel < totalNumOutputChannels; channel++) {
		for (int sample = 0; sample < numSamples; sample++) {
			outputChannelData[channel][sample] *= _globalVolume;
		}
	}
}
void OPAudioVolume::setGlobalVolume(float volume) {
	_globalVolume = volume;
}

void OPAudioVolume::resetGlobalVolumeDefault() {
	_globalVolume = 1;
}
