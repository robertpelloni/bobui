#pragma once

// Generic include catch-all
#include "juce_gui_basics/juce_gui_basics.h"

// Append mock declarations
namespace juce {
    typedef unsigned char uint8;

    struct MidiDeviceInfo {
        std::string name;
        std::string identifier;
    };

    class MidiMessage {
    public:
        static MidiMessage noteOn(int channel, int noteNumber, uint8 velocity);
        static MidiMessage noteOff(int channel, int noteNumber);
    };

    class MidiOutput {
    public:
        static std::vector<MidiDeviceInfo> getAvailableDevices();
        static std::unique_ptr<MidiOutput> openDevice(const std::string& identifier);
        void sendMessageNow(const MidiMessage& message);
    };

    namespace dsp {
        class StateVariableTPTFilter {
        public:
            void setType(int type);
            void setCutoffFrequency(double freq);
            void setResonance(double res);
            void process(float* left, float* right, int numSamples);
        };
    }
}
