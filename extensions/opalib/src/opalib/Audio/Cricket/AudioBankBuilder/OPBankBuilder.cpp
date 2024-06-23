//
//  OPBank.cpp
//  spk-talles2-mobile
//
//  Created by Arthur Motelevicz on 27/03/20.
//


#undef min
#undef max

// template instantiations
#include "ck/core/array.cpp"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

using namespace Cki;
////////////////////////////////////////

#include "OPBankBuilder.hpp"
#include <cocos/platform/CCFileUtils.h>

void OPBankBuilder::logFunc(CkLogType type, const char* msg)
{
    if (type == kCkLog_Error || type == kCkLog_Warning)
    {
        const char* label;
        switch (type)
        {
            case kCkLog_Info:    label = "INFO";    break;
            case kCkLog_Warning: label = "WARNING"; break;
            case kCkLog_Error:   label = "ERROR";   break;
            default: return;
        }
        CK_PRINT("%s: %s\n", label, msg);
    }
}


////////////////////////////////////////
// audio formats

struct Format
{
    const char* name;
    AudioFormat::Value value;
};

Format g_formats[] =
{
    { "pcm16", AudioFormat::k_pcmI16 },
    { "pcm8", AudioFormat::k_pcmI8 },
    { "adpcm", AudioFormat::k_adpcm },
};

AudioFormat::Value OPBankBuilder::parseFormat(const char* str)
{
    String formatStr = str;
    formatStr.makeLowerCase();

    for (int i = 0; i < CK_ARRAY_COUNT(g_formats); ++i)
    {
        if (!strcmp(str, g_formats[i].name))
        {
            return g_formats[i].value;
        }
    }

    return AudioFormat::k_unknown;
}

const char* OPBankBuilder::getFormatName(AudioFormat::Value format)
{
    for (int i = 0; i < CK_ARRAY_COUNT(g_formats); ++i)
    {
        if (format == g_formats[i].value)
        {
            return g_formats[i].name;
        }
    }

    return NULL;
}

////////////////////////////////////////
// attributes

// bank sounds
const char* k_nameAttr      = "name";
const char* k_sourceAttr    = "source";

// sampleInfo
const char* k_formatAttr    = "format";
const char* k_volumeAttr    = "volume";
const char* k_panAttr       = "pan";
const char* k_loopStartAttr = "loopStart";
const char* k_loopEndAttr   = "loopEnd";
const char* k_loopCountAttr = "loopCount";


////////////////////////////////////////

bool OPBankBuilder::buildSampleInfo(SampleInfo& sampleInfo, const char* path, const TiXmlElement* soundElem, Encoder*& encoder, AudioReader*& reader)
{
    if (!FileStream::exists(path))
    {
        CK_PRINT("  ERROR: %s: no such file\n", path);
        return false;
    }

    reader = AudioReader::create(path);
    if (!reader)
    {
        CK_PRINT("  ERROR: %s: invalid audio file!\n", path);
        return false;
    }

    if (reader->getChannels() > 2)
    {
        CK_PRINT("  ERROR: %s: contains %d channels; maximum is 2\n", path, reader->getChannels());
        delete reader;
        return false;
    }

    // format
    AudioFormat::Value format = AudioFormat::k_pcmI16;
    const char* formatStr = soundElem->Attribute(k_formatAttr);
    if (formatStr)
    {
        format = parseFormat(formatStr);
        if (format == AudioFormat::k_unknown)
        {
            String s;
            for (int i = 0; i < CK_ARRAY_COUNT(g_formats); ++i)
            {
                s.append(g_formats[i].name);
                s.append(" ");
            }
            CK_PRINT("  WARNING: unknown format \"%s\" specified; will use pcm16 instead.  (Possible formats: %s)\n", formatStr, s.getBuffer());
            format = AudioFormat::k_pcmI16;
        }
    }
    CK_PRINT("  format: %s\n", getFormatName(format));

    // volume
    const char* volumeStr = soundElem->Attribute(k_volumeAttr);
    float volume = 1.0f;
    if (volumeStr)
    {
        if (sscanf(volumeStr, "%f", &volume) != 1)
        {
            CK_PRINT("  WARNING: could not parse volume \"%s\"\n", volumeStr);
        }
    }

    // pan
    const char* panStr = soundElem->Attribute(k_panAttr);
    float pan = 0.0f;
    if (panStr)
    {
        if (sscanf(panStr, "%f", &pan) != 1)
        {
            CK_PRINT("  WARNING: could not parse pan \"%s\"\n", panStr);
        }
    }


    // loops
    int loopStart = 0;
    int loopEnd = -1;
    int loopCount = 0;

    // loop start/end:
    //  if loop start/end are specified as attributes, use them;
    //  else if there is a loop in the file, use its start & end;
    //  else if there is a region called "loop", use its start & end.
    // loop count:
    //  if loop count is specified as attribute, use it;
    //  else if there is a loop in the file, use its count.
    //
    if (reader->getNumLoops() > 0)
    {
        reader->getLoopInfo(0, NULL, loopStart, loopEnd, loopCount);
    }
    else if (reader->getNumMarkers() > 0)
    {
        for (int i = 0; i < reader->getNumMarkers(); ++i)
        {
            const char* name;
            int start, end;
            reader->getMarkerInfo(i, &name, start, end);
            if (name && String::equals(name, "loop", -1, true) && start != end)
            {
                loopStart = start;
                loopEnd = end;
                loopCount = -1;
                break;
            }
        }
    }


    // loop start (default 0)
    const char* loopStartStr = soundElem->Attribute(k_loopStartAttr);
    if (loopStartStr)
    {
        if (sscanf(loopStartStr, "%d", &loopStart) != 1)
        {
            CK_PRINT("  WARNING: could not parse loop start \"%s\"\n", loopStartStr);
        }
        else
        {
            if (loopCount == 0)
            {
                loopCount = -1;
            }
        }
    }

    // loop end (default end)
    const char* loopEndStr = soundElem->Attribute(k_loopEndAttr);
    if (loopEndStr)
    {
        if (sscanf(loopEndStr, "%d", &loopEnd) != 1)
        {
            CK_PRINT("  WARNING: could not parse loop end \"%s\"\n", loopEndStr);
        }
        else
        {
            if (loopCount == 0)
            {
                loopCount = -1;
            }
        }
    }

    // loop count (default 0, or -1 if loop start or end were set)
    const char* loopCountStr = soundElem->Attribute(k_loopCountAttr);
    if (loopCountStr)
    {
        if (sscanf(loopCountStr, "%d", &loopCount) != 1)
        {
            CK_PRINT("  WARNING: could not parse loop count \"%s\"\n", loopCountStr);
        }
    }

    encoder = Encoder::create(format, reader->getChannels());

    // actual frames must be multiple of blocks
    int inFrames = reader->getFrames();
    int framesPerBlock = encoder->getFramesPerBlock();
    int blocks = (inFrames + framesPerBlock - 1) / framesPerBlock;
    int outFrames = blocks * framesPerBlock;
    CK_ASSERT(outFrames >= inFrames);
    if (outFrames != inFrames)
    {
        CK_PRINT("  padding with %d sample frames of silence at end to fill last block\n", outFrames - inFrames);
    }

    // fix loops
    if (loopStart < 0)
    {
        loopStart = 0;
    }
    if (loopStart >= outFrames)
    {
        loopStart = (blocks-1) * framesPerBlock;
    }

    if (loopEnd < 0 || loopEnd > outFrames)
    {
        loopEnd = outFrames;
    }
    if (loopEnd <= loopStart)
    {
        loopEnd = loopStart + 1;
    }

    if (loopCount < -1)
    {
        loopCount = -1;
    }

    if (loopCount != 0)
    {
        CK_PRINT("  loop start: %d\n", loopStart);
        CK_PRINT("  loop end: %d\n", loopEnd);
        CK_PRINT("  loop count: %d\n", loopCount);
    }

    sampleInfo.format = format;
    sampleInfo.channels = reader->getChannels();
    sampleInfo.sampleRate = reader->getSampleRate();
    sampleInfo.blocks = blocks;
    sampleInfo.blockBytes = encoder->getBytesPerBlock();
    sampleInfo.blockFrames = encoder->getFramesPerBlock();
    sampleInfo.volume = (uint16) (Math::clamp(volume, 0.0f, 1.0f) * CK_UINT16_MAX);
    sampleInfo.pan = (int16) (Math::clamp(pan, -1.0f, 1.0f) * CK_INT16_MAX);
    sampleInfo.loopStart = (uint32) loopStart;
    sampleInfo.loopEnd = (uint32) loopEnd;
    sampleInfo.loopCount = (int16) loopCount;

    return true;
}



////////////////////////////////////////
// bank

bool OPBankBuilder::buildSample(Sample& sample, const TiXmlElement* soundElem, const char* inDir)
{
    // name
    const char* name = soundElem->Attribute(k_nameAttr);
    if (name)
    {
        sample.name = name;
        CK_PRINT("  name: %s\n", name);
    }
    else
    {
        CK_PRINT("  WARNING: no name specified\n");
    }

    // source
    const char* source = soundElem->Attribute(k_sourceAttr);
    if (!source)
    {
        CK_PRINT("  ERROR: no source specified\n");
        return false;
    }
    CK_PRINT("  source: %s\n", source);

    Path sourcePath(source);
    if (sourcePath.isRelative())
    {
        sourcePath = inDir;
        sourcePath.appendChild(source);
    }


    Encoder* encoder;
    AudioReader* reader;
    if (buildSampleInfo(sample.info, sourcePath.getBuffer(), soundElem, encoder, reader))
    {
        // read source data
        int outFrames = sample.info.blocks * sample.info.blockFrames;
        int outSamples = outFrames * sample.info.channels;
        int inFrames = reader->getFrames();
        int inSamples = inFrames * sample.info.channels;

        int16* srcSamples = new int16[outSamples];
        reader->read(srcSamples, inSamples);
        delete reader;

        // pad with zeroes
        for (int i = inSamples; i < outSamples; ++i)
        {
            srcSamples[i] = 0;
        }

        // allocate destination data (assume size <= source data)
        Cki::byte* destSamples = new Cki::byte[outSamples * sizeof(int16)];

        sample.dataSize = encoder->encode(srcSamples, destSamples, outFrames, true);
        sample.data = destSamples;
        CK_ASSERT(sample.dataSize == sample.info.blocks * sample.info.blockBytes);

        delete encoder;
        delete[] srcSamples;

        return true;
    }
    else
    {
        return false;
    }
}


bool OPBankBuilder::buildBank(TiXmlDocument& bankDoc, const char* inDir, const char* outPath, bool single)
{
    // TODO: check for unknown elements & attributes, which might be accidentaly misspellings

    BankData bank;

    Array<Cki::byte*> buffers;

    // parse root bank element
    TiXmlElement* bankElem = bankDoc.FirstChildElement("bank");
    if (!bankElem)
    {
        CK_PRINT("ERROR: not a bank file\n");
        return false;
    }
    const char* name = bankElem->Attribute("name");
    if (name)
    {
        bank.setName(name);
    }
    else
    {
        CK_PRINT("WARNING: no bank name specified\n");
    }
    CK_PRINT("bank name: %s\n", bank.getName().getBuffer());

    // parse sound elements
    TiXmlElement* soundElem = bankElem->FirstChildElement("sound");
    if (!soundElem)
    {
        CK_PRINT("WARNING: no sounds in bank\n");
    }

    int index = -1;
    while (soundElem)
    {
        ++index;
        CK_PRINT("adding sample %d\n", index);

        Sample sample;

        if (buildSample(sample, soundElem, inDir))
        {
            bank.addSample(sample);
            buffers.append(sample.data);
        }
        else
        {
            if (single)
            {
                // don't write bank file, since this is the only sound and it failed
                return false;
            }
        }

        soundElem = soundElem->NextSiblingElement("sound");
    }

    bool success;

    // TODO check for overwrite?
    FileStream outFile(outPath, FileStream::k_writeTruncate);
    if (outFile.isValid())
    {
        CK_PRINT("\n");
        CK_PRINT("writing to %s\n", outPath);
        SwapStream stream(outFile);

        FileHeader fileHeader;
        fileHeader.marker = FileHeader::k_marker;
        fileHeader.targets = Target::k_all;
        fileHeader.fileType = FileType::k_bank;
        fileHeader.fileVersion = FileHeader::k_version;
        stream << fileHeader;

        stream << bank;
        success = true;
    }
    else
    {
        CK_PRINT("ERROR: could not open %s for writing\n", outPath);
        success = false;
    }

    for (int i = 0; i < buffers.getSize(); ++i)
    {
        delete[] buffers[i];
    }

    return success;
}


// build from single input file
bool OPBankBuilder::buildBankFromAudioFile(const char* inFilename, const char* outFilename, TiXmlElement* soundElem)
{
    // if output file not specified, use input file with ".ckb" extension
    Path outPath(outFilename ? outFilename : inFilename);
    if (!outFilename)
    {
        outPath.setExtension("ckb");
    }


    /*
    file.wav:
    <bank name="file">
        <sound name="file" source="path/to/file.wav" format="format"/>
    </bank>
    */

    Path inPath(inFilename);
    inPath.setExtension(NULL);
    const char* name = inPath.getLeaf();

    TiXmlDocument bankDoc;
    TiXmlElement* rootElem = new TiXmlElement("bank");
    rootElem->SetAttribute("name", name);
    bankDoc.LinkEndChild(rootElem);

    soundElem->SetAttribute("name", name);
    soundElem->SetAttribute("source", inFilename);
    rootElem->LinkEndChild(soundElem);

    return buildBank(bankDoc, ".", outPath.getBuffer(), true);
}

// build from single input file
CkBank* OPBankBuilder::buildBankFromAudioFilePath(const char* inFilename)
{
    
    TiXmlElement* soundElem = new TiXmlElement("sound");

    Path inPath(inFilename);
    inPath.setExtension(NULL);
    const char* name = inPath.getLeaf();
    
    TiXmlDocument bankDoc;
    TiXmlElement* rootElem = new TiXmlElement("bank");
    rootElem->SetAttribute("name", name);
    bankDoc.LinkEndChild(rootElem);

    soundElem->SetAttribute("name", name);
    soundElem->SetAttribute("source", inFilename);
    rootElem->LinkEndChild(soundElem);

    auto bankData = buildBankData(bankDoc, ".");
    
    std::string tempAudioFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "/temp.ckb";

    Path outPath(tempAudioFilePath.c_str(),kCkPathType_FileSystem);
    
    CkBank* finalBank = NULL;
    
    auto outPathBuffer = outPath.getBuffer();
    
    FileStream outFile(outPathBuffer, FileStream::k_writeTruncate);
    if (outFile.isValid())
    {
        CK_PRINT("\n");
        CK_PRINT("writing to %s\n", outPathBuffer);
        SwapStream stream(outFile);

        FileHeader fileHeader;
        fileHeader.marker = FileHeader::k_marker;
        fileHeader.targets = Target::k_all;
        fileHeader.fileType = FileType::k_bank;
        fileHeader.fileVersion = FileHeader::k_version;
        stream << fileHeader;

        stream << *bankData;
        finalBank = CkBank::newBank(tempAudioFilePath.c_str(),kCkPathType_FileSystem);
    }
    
    return finalBank;
}


BankData* OPBankBuilder::buildBankData(TiXmlDocument& bankDoc, const char* inDir)
{
    BankData* bank = new BankData();

    Array<Cki::byte*> buffers;

    // parse root bank element
    TiXmlElement* bankElem = bankDoc.FirstChildElement("bank");
    if (!bankElem)
    {
        CK_PRINT("ERROR: not a bank file\n");
        return NULL;
    }
    const char* name = bankElem->Attribute("name");
    if (name)
    {
        bank->setName(name);
    }
    else
    {
        CK_PRINT("WARNING: no bank name specified\n");
    }
    CK_PRINT("bank name: %s\n", bank->getName().getBuffer());

    // parse sound elements
    TiXmlElement* soundElem = bankElem->FirstChildElement("sound");
    if (!soundElem)
    {
        CK_PRINT("WARNING: no sounds in bank\n");
    }

    int index = -1;
    while (soundElem)
    {
        ++index;
        CK_PRINT("adding sample %d\n", index);

        Sample sample;

        if (buildSample(sample, soundElem, inDir))
        {
            bank->addSample(sample);
            buffers.append(sample.data);
        }
        else
        {
            return NULL;
        }

        soundElem = soundElem->NextSiblingElement("sound");
    }

    return bank;
}


bool OPBankBuilder::buildBankFromXml(const char* inFilename, const char* outFilename)
{
    CK_PRINT("building bank from %s\n", inFilename);

    // if output file not specified, use input file with ".ckb" extension
    Path outPath(outFilename ? outFilename : inFilename);
    if (!outFilename)
    {
        outPath.setExtension("ckb");
    }

    // use input file dir as base for relative paths in bank xml
    Path inDir(inFilename);
    inDir.setParent();

    if (!FileStream::exists(inFilename))
    {
        CK_PRINT("ERROR: %s: no such file\n", inFilename);
        return false;
    }

    TiXmlDocument bankDoc(inFilename);
    if (!bankDoc.LoadFile())
    {
        CK_PRINT("ERROR: %s: invalid xml file\n", inFilename);
        return false;
    }

    return buildBank(bankDoc, inDir.getBuffer(), outPath.getBuffer(), false);
}


////////////////////////////////////////
// extract

bool OPBankBuilder::extract(const Path& outPath, const SampleInfo& sampleInfo, const Cki::byte* data, int bytes)
{
    int frames = sampleInfo.blocks * sampleInfo.blockFrames;
    int outSamples = sampleInfo.channels * frames;
    float* outBuf = new float[outSamples];

    switch (sampleInfo.format)
    {
        case AudioFormat::k_pcmI16:
            AudioUtil::convert((const int16*) data, outBuf, outSamples);
            break;

        case AudioFormat::k_pcmI8:
            AudioUtil::convert((const int8*) data, outBuf, outSamples);
            break;

        case AudioFormat::k_adpcm:
            {
                const Cki::byte* pIn = data;
                int16* pOut = (int16*) outBuf;
                for (int i = 0; i < sampleInfo.blocks; ++i)
                {
                    Adpcm::decode(pIn, Adpcm::k_bytesPerBlock, pOut, sampleInfo.channels);
                    pIn += Adpcm::k_bytesPerBlock;
                    if (sampleInfo.channels == 2)
                    {
                        Adpcm::decode(pIn, Adpcm::k_bytesPerBlock, pOut+1, sampleInfo.channels);
                        pIn += Adpcm::k_bytesPerBlock;
                    }
                    pOut += Adpcm::k_samplesPerBlock * sampleInfo.channels;
                }
                AudioUtil::convert((const int16*) outBuf, outBuf, outSamples);
            }
            break;

        case AudioFormat::k_pcmF32:
            AudioUtil::convert((const float*) data, outBuf, outSamples);
            break;

        default:
            CK_PRINT("ERROR: unknown sample format: %d\n", sampleInfo.format);
            delete[] outBuf;
            return false;
    }

    WavWriter writer(outPath.getBuffer(), sampleInfo.channels, sampleInfo.sampleRate, false);
    if (!writer.isValid())
    {
        CK_PRINT("ERROR: could not open %s for writing\n", outPath.getBuffer());
        return false;
    }

    CK_PRINT("writing %s\n", outPath.getBuffer());
    writer.write(outBuf, outSamples);
    return true;
}

////////////////////////////////////////


bool OPBankBuilder::buildStream(const char* inFilename, const char* outFilename, TiXmlElement* soundElem)
{
    CK_PRINT("building stream from %s\n", inFilename);

    TiXmlDocument streamDoc;
    streamDoc.LinkEndChild(soundElem);

    SampleInfo sampleInfo;

    Encoder* encoder;
    AudioReader* reader;
    if (buildSampleInfo(sampleInfo, inFilename, soundElem, encoder, reader))
    {
        // if output file not specified, use input file with ".cks" extension
        // TODO test for overwriting?
        Path outPath(outFilename ? outFilename : inFilename);
        if (!outFilename)
        {
            outPath.setExtension("cks");
        }

        FileStream outFile(outPath.getBuffer(), FileStream::k_writeTruncate);
        if (!outFile.isValid())
        {
            CK_PRINT("ERROR: could not open %s for writing\n", outPath.getBuffer());
            return false;
        }

        CK_PRINT("\n");
        CK_PRINT("writing to %s\n", outPath.getBuffer());

        SwapStream stream(outFile);

        FileHeader fileHeader;
        fileHeader.marker = FileHeader::k_marker;
        fileHeader.targets = Target::k_all;
        fileHeader.fileType = FileType::k_stream;
        fileHeader.fileVersion = FileHeader::k_version;
        stream << fileHeader;

        stream << sampleInfo;

        // dump out data
        const int k_bufSamples = 1024;
        int16 srcSamples[k_bufSamples];
        int16 destSamples[k_bufSamples];

        int dataBytes = 0;
        while (!reader->isAtEnd())
        {
            int samples = reader->read(srcSamples, k_bufSamples);
            int bytes = encoder->encode(srcSamples, (Cki::byte*) destSamples, samples/sampleInfo.channels, reader->isAtEnd());
            outFile.write(destSamples, bytes);
            dataBytes += bytes;
        }
        CK_ASSERT(dataBytes == sampleInfo.blocks * encoder->getBytesPerBlock());

        delete encoder;
        delete reader;

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////

// print info about a stream

bool OPBankBuilder::info(const char* path)
{
    if (!FileStream::exists(path))
    {
        CK_PRINT("ERROR: %s: no such file\n", path);
        return false;
    }

    FileStream file(path, FileStream::k_read);
    if (!file.isValid())
    {
        CK_PRINT("ERROR: %s: invalid file!\n", path);
        return false;
    }

    SwapStream stream(file);

    FileHeader fileHeader;
    stream >> fileHeader;

    if (fileHeader.marker != FileHeader::k_marker)
    {
        CK_PRINT("ERROR: invalid file marker! (%.4s)\n", fileHeader.marker.getData());
        return false;
    }
    if (fileHeader.fileVersion != FileHeader::k_version)
    {
        CK_PRINT("ERROR: invalid file version: %d (expecting %d)\n", fileHeader.fileVersion, FileHeader::k_version);
        return false;
    }

    CK_PRINT("platforms: ");
    for (int i = 0; i < sizeof(fileHeader.targets) * 8; ++i)
    {
        if ((1 << i) & fileHeader.targets)
        {
            const char* name = Target::getName((Target::Value) (1 << i));
            CK_PRINT("%s ", name ? name : "(unknown)");
        }
    }
    CK_PRINT("\n");
    if (fileHeader.targets & ~Target::k_all)
    {
        CK_PRINT("unknown platforms! (0x%x)\n", fileHeader.targets);
    }

    CK_PRINT("file type: %d\n", fileHeader.fileType);

    CK_PRINT("\n");
    if (fileHeader.fileType == FileType::k_stream)
    {
        CK_PRINT("stream info:\n");
        SampleInfo sampleInfo;
        stream >> sampleInfo;

        int frames = sampleInfo.blocks * sampleInfo.blockFrames;

        CK_PRINT("format:      %d\n", (int) sampleInfo.format);
        CK_PRINT("channels:    %d\n", (int) sampleInfo.channels);
        CK_PRINT("sample rate: %d\n", (int) sampleInfo.sampleRate);
        CK_PRINT("frames:      %d\n", frames);
        CK_PRINT("duration:    %f s\n", (float) frames / sampleInfo.sampleRate);
        CK_PRINT("volume:      %f\n", (float) sampleInfo.volume / CK_UINT16_MAX);
        CK_PRINT("pan:         %f\n", (float) sampleInfo.pan / CK_INT16_MAX);
        CK_PRINT("loop:        %d,%d\n", (int) sampleInfo.loopStart, (int) sampleInfo.loopEnd);
        CK_PRINT("loop count:  %d\n", (int) sampleInfo.loopCount);
    }
    else if (fileHeader.fileType == FileType::k_bank)
    {
        Bank* bank = Bank::newBank(path, kCkPathType_FileSystem, 0, 0);
        if (bank && !bank->isFailed())
        {
            CK_PRINT("bank name: %s\n", bank->getName());
            for (int i = 0; i < bank->getNumSounds(); ++i)
            {
                const Sample& sample = bank->getSample(i);
                int frames = sample.info.blocks * sample.info.blockFrames;
                CK_PRINT("\n");
                CK_PRINT("  sound %d:\n", i);
                CK_PRINT("      name:        %s\n", sample.name.getBuffer());
                CK_PRINT("      format:      %d\n", (int) sample.info.format);
                CK_PRINT("      channels:    %d\n", (int) sample.info.channels);
                CK_PRINT("      sample rate: %d\n", (int) sample.info.sampleRate);
                CK_PRINT("      frames:      %d\n", frames);
                CK_PRINT("      bytes:       %d\n", sample.dataSize);
                CK_PRINT("      duration:    %f s\n", (float) frames / sample.info.sampleRate);
                CK_PRINT("      volume:      %f\n", (float) sample.info.volume / CK_UINT16_MAX);
                CK_PRINT("      pan:         %f\n", (float) sample.info.pan / CK_INT16_MAX);
                CK_PRINT("      loop:        %d,%d\n", (int) sample.info.loopStart, (int) sample.info.loopEnd);
                CK_PRINT("      loop count:  %d\n", (int) sample.info.loopCount);
            }
            bank->destroy();
        }
    }

    return true;
}

////////////////////////////////////////

// extract sounds

bool OPBankBuilder::extract(const char* path)
{
    if (!FileStream::exists(path))
    {
        CK_PRINT("ERROR: %s: no such file\n", path);
        return false;
    }

    FileStream inFile(path, FileStream::k_read);
    if (!inFile.isValid())
    {
        CK_PRINT("ERROR: %s: invalid file!\n", path);
        return false;
    }

    SwapStream stream(inFile);

    FileHeader fileHeader;
    stream >> fileHeader;

    if (fileHeader.marker != FileHeader::k_marker)
    {
        CK_PRINT("ERROR: invalid file marker! (%.4s)\n", fileHeader.marker.getData());
        return false;
    }
    if (fileHeader.fileVersion != FileHeader::k_version)
    {
        CK_PRINT("ERROR: invalid file version: %d (expecting %d)\n", fileHeader.fileVersion, FileHeader::k_version);
        return false;
    }

    if (fileHeader.fileType == FileType::k_stream)
    {
        SampleInfo sampleInfo;
        stream >> sampleInfo;

        // TODO test for overwriting?
        Path outPath(path);
        outPath.setExtension(NULL);
        outPath.append("_extracted");
        outPath.setExtension("wav");

        int inBytes = inFile.getSize() - inFile.getPos();
        Cki::byte* inBuf = new Cki::byte[inBytes];
        inFile.read(inBuf, inBytes);

        bool result = extract(outPath, sampleInfo, inBuf, inBytes);
        delete[] inBuf;
        if (!result)
        {
            return false;
        }
    }
    else if (fileHeader.fileType == FileType::k_bank)
    {
        Bank* bank = Bank::newBank(path, kCkPathType_FileSystem, 0, 0);
        if (bank && !bank->isFailed())
        {
            CK_PRINT("bank name: %s\n", bank->getName());
            for (int i = 0; i < bank->getNumSounds(); ++i)
            {
                const Sample& sample = bank->getSample(i);

                // TODO test for overwriting?
                Path outPath(path);
                outPath.setExtension(NULL);
                outPath.append("_");
                outPath.append(sample.name.getBuffer());
                outPath.append("_extracted");
                outPath.setExtension("wav");

                extract(outPath, sample.info, sample.data, sample.dataSize);
            }
            bank->destroy();
        }
    }

    return true;
}

////////////////////////////////////////

void OPBankBuilder::printUsage(const char* app)
{
    Path appPath(app);
    const char* toolName = appPath.getLeaf();
    CK_PRINT("\n");
    CK_PRINT("Usage:\n");
    CK_PRINT("\n");
    CK_PRINT(" %s buildbank   [common_options] <infile> [<outfile>]\n", toolName);
    CK_PRINT("   builds a bank file (.ckb) from an xml bank description file (.ckbx)\n");
    CK_PRINT("\n");
    CK_PRINT(" %s buildbank   [common_options] -single [sound_attributes] <infile> [<outfile>]\n", toolName);
    CK_PRINT("   builds a bank file (.ckb) containing one sound\n");
    CK_PRINT("\n");
    CK_PRINT(" %s buildstream [common_options] [sound_attributes] <infile> [<outfile>]\n", toolName);
    CK_PRINT("   builds a stream file (.cks)\n");
    CK_PRINT("\n");
    CK_PRINT(" %s info [common_options] <file>\n", toolName);
    CK_PRINT("   prints info about a bank (.ckb) or stream (.cks) file\n");
    CK_PRINT("\n");
    CK_PRINT(" %s extract [common_options] <infile>\n", toolName);
    CK_PRINT("   extracts sounds from bank (.ckb) or stream (.cks) into .wav files\n");
    CK_PRINT("\n");
    CK_PRINT("\n");
    CK_PRINT("\n");
    CK_PRINT("Available common options are:\n");
    CK_PRINT("  -verbose\n");
    CK_PRINT("\n");
    CK_PRINT("Available sound attribute options are:\n");
    CK_PRINT("  -format <format>\n");
    CK_PRINT("  -volume <volume>\n");
    CK_PRINT("  -pan <pan>\n");
    CK_PRINT("  -loopStart <frame>\n");
    CK_PRINT("  -loopEnd <frame>\n");
    CK_PRINT("  -loopCount <count>\n");
    CK_PRINT("\n");
}