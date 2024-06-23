/*
  ==============================================================================

    OPMidi.cpp
    Created: 6 Jun 2020 3:18:22pm
    Author:  Arthur Motelevicz

  ==============================================================================
*/

#include "OPMidi.h"
using namespace juce;

OPMidi::OPMidi()
{
    // init juce system if not initialized
    MessageManager::getInstance();

    startTimer(3000);
}

void OPMidi::timerCallback()
{
    // update input list
    updateAvailableDevices(true);

    // update output list
    updateAvailableDevices(false);

    // print all inputs and outputs
#if COCOS2D_DEBUG
    printAvailableDevices(true);
    printAvailableDevices(false);
#endif

    openLaunchpadConnection();
}

void OPMidi::updateAvailableDevices(bool isInput)
{
    auto availableDevices = isInput ? MidiInput::getAvailableDevices() : MidiOutput::getAvailableDevices();

    closeUnpluggedDevices(availableDevices, isInput);

    ReferenceCountedArray<MidiDeviceListEntry> newDeviceList;

    for (auto& newDevice : availableDevices) {
        MidiDeviceListEntry::Ptr entry = findDevice(newDevice, isInput);

        if (entry == nullptr) entry = new MidiDeviceListEntry(newDevice);

        newDeviceList.add(entry);
    }

    ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = isInput ? midiInputs : midiOutputs;
    midiDevices = newDeviceList;

    //    openLaunchpadConnection();
}

ReferenceCountedObjectPtr<MidiDeviceListEntry> OPMidi::findDevice(MidiDeviceInfo device, bool isInput) const
{
    const ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = isInput ? midiInputs : midiOutputs;

    for (auto& d : midiDevices)
        if (d->deviceInfo == device) return d;

    return nullptr;
}

void OPMidi::printAvailableDevices(bool isInput)
{
    ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = isInput ? midiInputs : midiOutputs;

    DBG(String(isInput ? "input" : "output"));

    for (auto device : midiDevices) {
        DBG(" Name: " + device->deviceInfo.name + " Identifier: " + device->deviceInfo.identifier);
    }
}

void OPMidi::openLaunchpadConnection()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    openLaunchpadConnectionIOS();
#else
    openLaunchpadConnectionAndroid();
#endif
}

void OPMidi::openLaunchpadConnectionIOS()
{
    if (_isLaunchpadConnected) return;

    int totalDevices = 0;

    for (int i = 1; i < midiInputs.size(); i++) {
        auto nameLowered = midiInputs[i]->deviceInfo.name.toLowerCase();
        // launchapd x, pro mk3, mini mk3
        if (nameLowered.contains("pro mk3") || nameLowered.contains("x") || nameLowered.contains("mini mk3")) {
            // connect only on midi port
            if (nameLowered.contains("midi")) {
                openDevice(i, true);
                totalDevices++;
            }
            // launchapd pro
        } else if (nameLowered.contains("pro")) {
            // connect on live port
            if (nameLowered.contains("live")) {
                openDevice(i, true);
                totalDevices++;
            }
            // launchpad mk2, launchpad mini, launchpad s, launchpad
        } else if (nameLowered.contains("launchpad")) {
            openDevice(i, true);
            totalDevices++;
        }
    }

    for (int i = 1; i < midiOutputs.size(); i++) {
        auto nameLowered = midiOutputs[i]->deviceInfo.name.toLowerCase();
        // launchapd x, pro mk3, mini mk3
        if (nameLowered.contains("pro mk3") || nameLowered.contains("x") || nameLowered.contains("mini mk3")) {
            // connect only on midi port
            if (nameLowered.contains("midi")) {
                openDevice(i, false);
                totalDevices++;
            }
            // launchapd pro
        } else if (nameLowered.contains("pro")) {
            // connect on live port
            if (nameLowered.contains("live")) {
                openDevice(i, false);
                totalDevices++;
            }
            // launchpad mk2, launchpad mini, launchpad s, launchpad
        } else if (nameLowered.contains("launchpad")) {
            openDevice(i, false);
            totalDevices++;
        }
    }

    updateConnectionState(totalDevices > 0);
}

void OPMidi::openLaunchpadConnectionAndroid()
{
    if (_isLaunchpadConnected) return;

    std::map<String, int> midiDevices;

    for (const auto& midiOutput : midiOutputs) {
        midiDevices[midiOutput->deviceInfo.name] += 1;
    }

    for (const auto& midiInput : midiInputs) {
        midiDevices[midiInput->deviceInfo.name] += 1;
    }

    int totalDevices = 0;

    for (const auto& device : midiDevices) {
        if (device.second == 2) {
            totalDevices++;
            for (int i = 0; i < midiInputs.size(); i++) {
                if (midiInputs[i]->deviceInfo.name == device.first) {
                    openDevice(i, true);
                }
            }

            for (int i = 0; i < midiOutputs.size(); i++) {
                if (midiOutputs[i]->deviceInfo.name == device.first) {
                    openDevice(i, false);
                }
            }
        }
    }

    updateConnectionState(totalDevices > 0);
}

void OPMidi::closeUnpluggedDevices(const Array<MidiDeviceInfo>& currentlyPluggedInDevices, bool isInput)
{
    ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = isInput ? midiInputs : midiOutputs;

    for (auto i = midiDevices.size(); --i >= 0;) {
        auto& d = *midiDevices[i];

        if (!currentlyPluggedInDevices.contains(d.deviceInfo)) {
            if (isInput ? d.inDevice.get() != nullptr : d.outDevice.get() != nullptr) {
                closeDevice(i, isInput);
                updateConnectionState(false);
            }

            midiDevices.remove(i);
        }
    }
}

bool OPMidi::openDevice(int index, bool isInput)
{
    if (isInput) {
        midiInputs[index]->inDevice = MidiInput::openDevice(midiInputs[index]->deviceInfo.identifier, this);

        if (midiInputs[index]->inDevice.get() == nullptr) {
            DBG("MidiDemo::openDevice: open input device for index = " << index << " failed!");
            return false;
        }

        midiInputs[index]->inDevice->start();
        return true;
    } else {
        midiOutputs[index]->outDevice = MidiOutput::openDevice(midiOutputs[index]->deviceInfo.identifier);

        if (midiOutputs[index]->outDevice.get() == nullptr) {
            DBG("MidiDemo::openDevice: open output device for index = " << index << " failed!");
            return false;
        }

        return true;
    }
}

void OPMidi::closeDevice(int index, bool isInput)
{
    if (isInput) {
        midiInputs[index]->inDevice->stop();
        midiInputs[index]->inDevice.reset();
    } else {
        midiOutputs[index]->outDevice.reset();
    }
}

//==============================================================================
void OPMidi::handleIncomingMidiMessage(MidiInput* /*source*/, const MidiMessage& message)
{
    // This is called on the MIDI thread
    const ScopedLock sl(midiMonitorLock);
    incomingMessages.add(message);
    triggerAsyncUpdate();
}

void OPMidi::handleAsyncUpdate()
{
    // This is called on the message loop
    Array<MidiMessage> messages;

    const ScopedLock sl(midiMonitorLock);
    messages.swapWith(incomingMessages);

    String messageText;

    for (auto& m : messages) {
        //       messageText << "Note: " << m.getNoteNumber() << "Vel: " <<  m.getVelocity() << " Contrl: " << m.getControllerNumber()  << " \n";
        if (_onNotecallback) {
            if (m.isController()) {
                _onNotecallback(m.getNoteNumber(), 127);
            } else if (m.isNoteOn() || m.isNoteOff()) {
                _onNotecallback(m.getNoteNumber(), m.getVelocity());
            }
        }
    }
}

void OPMidi::sendToOutputs(const MidiMessage& msg)
{
    for (auto midiOutput : midiOutputs)
        if (midiOutput->outDevice != nullptr) midiOutput->outDevice->sendMessageNow(msg);
}

void OPMidi::sendNoteOn(int note, int velocity, int channel)
{
    if (_isLaunchpadConnected) {
        sendToOutputs(MidiMessage::noteOn(channel, note, (uint8)velocity));
    }
}

void OPMidi::sendNoteOff(int note, int channel)
{
    if (_isLaunchpadConnected) {
        sendToOutputs(MidiMessage::noteOff(channel, note));
    }
}

std::string OPMidi::getDeviceName()
{
    for (auto midiOutput : midiOutputs) {
        if (midiOutput->outDevice != nullptr) {
            return midiOutput->deviceInfo.name.toStdString();
        }
    }

    return "";
}

bool OPMidi::isDeviceConnected()
{
    return _isLaunchpadConnected;
}

void OPMidi::updateConnectionState(bool isConnected)
{
    if (_previousState == isConnected) return;

    _isLaunchpadConnected = isConnected;
    _previousState = _isLaunchpadConnected;

    if (_isLaunchpadConnected) {
        if (_onDeviceConnectCallback) _onDeviceConnectCallback();
    } else {
        if (_onDeviceDisconnectCallback) _onDeviceDisconnectCallback();
    }
}

OPMidi::~OPMidi()
{
    stopTimer();
    midiInputs.clear();
    midiOutputs.clear();
}