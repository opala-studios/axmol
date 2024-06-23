/*
  ==============================================================================

    OPMidi.h
    Created: 6 Jun 2020 3:18:22pm
    Author:  Arthur Motelevicz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cocos2d.h>

struct MidiDeviceListEntry : juce::ReferenceCountedObject
{
    MidiDeviceListEntry (juce::MidiDeviceInfo info) : deviceInfo (info) {}

    juce::MidiDeviceInfo deviceInfo;
    std::unique_ptr<juce::MidiInput> inDevice;
    std::unique_ptr<juce::MidiOutput> outDevice;

    using Ptr = juce::ReferenceCountedObjectPtr<MidiDeviceListEntry>;
};

class OPMidi :  private juce::Timer,
                private juce::MidiInputCallback,
                private juce::AsyncUpdater
{
    
public:
    typedef std::function<void(int note, int velocity)> OPMidi_OnNoteDeviceCallback;
    typedef std::function<void()> OPMidi_OnDeviceConnect;
    typedef std::function<void()> OPMidi_OnDeviceDisconnect;

    void setOnNoteDeviceCallback(OPMidi_OnNoteDeviceCallback value) { _onNotecallback = value; }
    void setOnDeviceConnectCallback(OPMidi_OnDeviceConnect value) { _onDeviceConnectCallback = value; }
    void setOnDeviceDisconnectCallback(OPMidi_OnDeviceDisconnect value) { _onDeviceDisconnectCallback = value; }

    void sendToOutputs (const juce::MidiMessage& msg);
    void sendNoteOn(int note, int velocity = 127, int channel = 1);
    void sendNoteOff(int note, int channel = 1);
    
    std::string getDeviceName();
    bool isDeviceConnected();

    OPMidi();
    ~OPMidi();
    
protected:

    void timerCallback() override;
    void updateAvailableDevices(bool isInput);
    bool openDevice(int index,bool isInput);
    void closeDevice(int index,bool isInput);

    //test funtion to open the last available device as the spl needs
    void openLaunchpadConnection();
    void openLaunchpadConnectionIOS();
    void openLaunchpadConnectionAndroid();

    bool _isLaunchpadConnected = false;
    bool _previousState = false;

    void closeUnpluggedDevices (const juce::Array<juce::MidiDeviceInfo>& currentlyPluggedInDevices, bool isInputDevice);


    juce::ReferenceCountedObjectPtr<MidiDeviceListEntry> findDevice (juce::MidiDeviceInfo device,bool isInput) const;
    juce::ReferenceCountedArray<MidiDeviceListEntry> midiInputs, midiOutputs;

    void printAvailableDevices(bool isInput);

    //MidiInputCallback protocol
    void handleIncomingMidiMessage (juce::MidiInput* /*source*/, const juce::MidiMessage& message) override;

    juce::CriticalSection midiMonitorLock;
    juce::Array<juce::MidiMessage> incomingMessages;

    //AsyncUpdater protocol
    void handleAsyncUpdate() override;

    OPMidi_OnNoteDeviceCallback _onNotecallback;
    OPMidi_OnDeviceConnect _onDeviceConnectCallback;
    OPMidi_OnDeviceDisconnect _onDeviceDisconnectCallback;

    void updateConnectionState(bool isConnected);
    
    
private:

    std::vector<std::pair<std::string, std::string>> _midiPortIdentifierMap; // [name, identifier]
    
};
