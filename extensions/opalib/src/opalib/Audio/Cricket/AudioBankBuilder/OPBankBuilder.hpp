//
//  OPBank.hpp
//  spk-talles2-mobile
//
//  Created by Arthur Motelevicz on 27/03/20.
//

#ifndef OPBankBuilder_hpp
#define OPBankBuilder_hpp

#include <stdio.h>
#include <cocos2d.h>

#include "ck/ck.h"
#include "ck/config.h"
#include "ck/core/debug.h"
#include "ck/core/path.h"
#include "ck/core/string.h"
#include "ck/core/filestream.h"
#include "ck/core/math.h"
#include "ck/core/swapstream.h"
#include "ck/core/version.h"
#include "ck/core/logger.h"
#include "ck/core/array.h"
#include "ck/core/system.h"
#include "ck/audio/fileheader.h"
#include "ck/audio/sampleinfo.h"
#include "ck/audio/sample.h"
#include "ck/audio/bankdata.h"
#include "ck/audio/bank.h"
#include "ck/audio/wavwriter.h"
#include "ck/audio/audioutil.h"


#include "encoder.h"
#include "argparser.h"
#include "audioreader.h"
#include "tinyxml.h"

class OPBankBuilder {

protected:

    static void logFunc(CkLogType type, const char* msg);
    static Cki::AudioFormat::Value parseFormat(const char* strstatic );
    static const char* getFormatName(Cki::AudioFormat::Value format);
    static bool buildSampleInfo(Cki::SampleInfo& sampleInfo, const char* path, const TiXmlElement* soundElem, Cki::Encoder*& encoder, Cki::AudioReader*& reader);
    static bool buildSample(Cki::Sample& sample, const TiXmlElement* soundElem, const char* inDir);
    static bool buildBank(TiXmlDocument& bankDoc, const char* inDir, const char* outPath, bool single);
    static bool buildBankFromAudioFile(const char* inFilename, const char* outFilename, TiXmlElement* soundElem);
    static bool buildBankFromXml(const char* inFilename, const char* outFilename);
    static bool extract(const Cki::Path& outPath, const Cki::SampleInfo& sampleInfo, const Cki::byte* data, int bytes);
    static bool buildStream(const char* inFilename, const char* outFilename, TiXmlElement* soundElem);
    static bool info(const char* path);
    static bool extract(const char* path);
    static void printUsage(const char* app);


    static Cki::BankData* buildBankData(TiXmlDocument& bankDoc, const char* inDir);
   
public:
    
    static CkBank* buildBankFromAudioFilePath(const char* inFilename);

};
#endif /* OPBankBuilder_hpp */
