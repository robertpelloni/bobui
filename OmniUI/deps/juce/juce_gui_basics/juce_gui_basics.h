#pragma once

// Mock JUCE Header for OmniUI compilation
namespace juce {

class Component {
public:
    virtual ~Component() {}
    virtual void paint(void* g) {}
    virtual void resized() {}

    void setBounds(int x, int y, int w, int h) {
        // Mock implementation
    }

    void setVisible(bool shouldBeVisible) {}
    void addToDesktop(int flags, void* nativeWindowHandle = nullptr) {}
};

void initialiseJuce_GUI();
void shutdownJuce_GUI();

class MessageManager {
public:
    static MessageManager* getInstance();
    void runDispatchLoop();
    void stopDispatchLoop();
};

}
