//
//  OPJuceHelper.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 14/10/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include <cocos2d.h>
#include "OPJuceHelper.hpp"
#include "../Misc/OPDirectory.hpp"

using namespace juce;

std::unique_ptr<AudioFormatReader> OPJuceHelper::createReaderFor(const std::string& filePath)
{
    juce::File file;

    std::unique_ptr<AudioFormatManager> formatManager { nullptr };
    std::unique_ptr<AudioFormatReader> reader { nullptr };
    
    formatManager = std::make_unique<juce::AudioFormatManager>();
//    formatManager->registerBasicFormats();
    formatManager->registerFormat (new WavAudioFormat(), true);
    formatManager->registerFormat (new AiffAudioFormat(), false);

   #if JUCE_USE_FLAC
    formatManager->registerFormat (new FlacAudioFormat(), false);
   #endif

   #if JUCE_USE_OGGVORBIS
    formatManager->registerFormat (new OggVorbisAudioFormat(), false);
   #endif
//
//   #if JUCE_MAC || JUCE_IOS
//    formatManager->registerFormat (new CoreAudioFormat(), false);
//   #endif
//
   #if JUCE_USE_MP3AUDIOFORMAT
    formatManager->registerFormat (new MP3AudioFormat(), false);
   #endif

    if(OPDirectory::isAssetsFolder(filePath))
    {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ZipFile apkZip (File::getSpecialLocation (File::invokedExecutableFile));
        std::unique_ptr<InputStream> inputStream(apkZip.createStreamForEntry (apkZip.getIndexOfFileName (filePath)));
        reader.reset(formatManager->createReaderFor (std::move(inputStream)));
    #else
        file = File().getChildFile(filePath);
//        auto inputStream = std::unique_ptr<OPFileInputStream>(new OPFileInputStream(file));
        reader.reset(formatManager->createReaderFor (file));
    #endif
    }
    else
    {
        file = File().getChildFile(filePath);
        reader.reset(formatManager->createReaderFor (file));
    }
    
    return reader;
}
