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
}
