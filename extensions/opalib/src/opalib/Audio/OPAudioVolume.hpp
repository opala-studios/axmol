//
//  OPAudioVolume.hpp
//  opalib-ios
//
//  Created by Opala Studios on 29/06/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPAudioVolume_hpp
#define OPAudioVolume_hpp

#include <Audio/OPBaseAudioIOCallback.h>

class OPAudioVolume : public OPBaseAudioIOCallback {
	public:
	
	void audioDeviceIOCallbackInt (
			const float** inputChannelData,
			int totalNumInputChannels,
			float** outputChannelData,
		    int totalNumOutputChannels,
		    int numSamples) override;
	
	void setGlobalVolume(float volume);
	void resetGlobalVolumeDefault();
	
	
	private:
	float _globalVolume = 1;
};

#endif /* OPAudioVolume_hpp */
