#include "JuceHeader.h"
#include <iostream>

// Mock Implementations for JUCE
namespace juce {
    void initialiseJuce_GUI() {
        std::cout << "[JUCE MOCK] Initializing JUCE GUI subsystem..." << std::endl;
    }

    void shutdownJuce_GUI() {
        std::cout << "[JUCE MOCK] Shutting down JUCE GUI subsystem..." << std::endl;
    }

    MessageManager* MessageManager::getInstance() { return nullptr; }
    void MessageManager::runDispatchLoop() {}
    void MessageManager::stopDispatchLoop() {}

    // Midi Mocks
    MidiMessage MidiMessage::noteOn(int channel, int noteNumber, uint8 velocity) { return MidiMessage(); }
    MidiMessage MidiMessage::noteOff(int channel, int noteNumber) { return MidiMessage(); }

    std::vector<MidiDeviceInfo> MidiOutput::getAvailableDevices() {
        return { {"Mock Synth A", "dev_1"}, {"Mock Synth B", "dev_2"} };
    }
    std::unique_ptr<MidiOutput> MidiOutput::openDevice(const std::string& identifier) {
        return std::make_unique<MidiOutput>();
    }
    void MidiOutput::sendMessageNow(const MidiMessage& message) {}

    // DSP Mocks
    namespace dsp {
        void StateVariableTPTFilter::setType(int type) {}
        void StateVariableTPTFilter::setCutoffFrequency(double freq) {}
        void StateVariableTPTFilter::setResonance(double res) {}
        void StateVariableTPTFilter::process(float* left, float* right, int numSamples) {}
    }
}
