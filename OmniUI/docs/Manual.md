# OmniUI User Manual & Architecture Guide

## 1. Philosophy
OmniUI aims to unify the fragmented C++ GUI landscape.
- **Qt** is chosen for its maturity, accessibility support, and massive widget ecosystem.
- **JUCE** is chosen for its superior audio handling, plugin hosting (VST/AU), and low-latency event processing.
- **TypeScript** is chosen to lower the barrier to entry, replacing the need for raw C++ or dynamic QML in high-level UI logic.

## 2. Architecture

### 2.1 The Event Loop Fusion
Typically, Qt and JUCE both want to own the main message loop. OmniUI resolves this by letting **Qt** drive the main loop (`QApplication::exec()`).
We inject the JUCE `MessageManager` into the Qt loop using a custom `QTimer` that pumps JUCE messages, or (on supported platforms) using native handle integration.

### 2.2 The `OmniApplication`
The entry point of any OmniUI app is `OmniApplication`.
```cpp
#include <OmniUI/OmniApp.h>

int main(int argc, char** argv) {
    OmniApplication app(argc, argv);
    app.initializeJuce(); // Bootstraps JUCE without starting its modal loop

    // Load main window
    app.loadMainSource("assets/main.ts");

    return app.exec();
}
```

### 2.3 `JuceWidget`
To render JUCE content inside Qt, we provide `JuceWidget`. This is a `QWidget` that:
1. Creates a native window handle (HWND/NSView/XID).
2. Attaches a `juce::Component` to that handle.
3. Forwards resize and focus events from Qt to JUCE.

## 3. TypeScript Integration

### 3.1 The Workflow
1. User writes `ui.ts`.
2. `omni build` runs `tsc` (TypeScript Compiler) to generate `ui.js`.
3. The C++ runtime loads `ui.js` into the QML Javascript Engine.
4. Signals and Slots are automatically bound.

### 3.2 Type Definitions
OmniUI provides `omni.d.ts` which exposes Qt types (like `QString`, `QWidget` proxies) to TypeScript.

## 4. WebAssembly (WASM) Build Pipeline

### 4.1 Overview
OmniUI uses Emscripten. The build system detects if `emcc` is present.
The `CMakeLists.txt` automatically links the static Qt for WASM libraries.

### 4.2 Threading
Since WASM threading is complex (SharedArrayBuffer requirements), OmniUI defaults to a single-threaded cooperative loop, but can be configured for pthreads if the host environment supports it (HTTPS + COOP/COEP headers).

## 5. Tooling (`omni` CLI)

The `omni` CLI is a Node.js wrapper around CMake and Ninja.
- `omni init <name>`: Scaffolds directory structure.
- `omni build --target=wasm`: Dockerizes the build if Emscripten is missing (optional), or runs local emcc.
- `omni generate widget <name>`: Scaffolds a new OmniUI QML widget.
- `omni generate audio <name>`: Scaffolds a new OmniUI audio processor.
- `omni test`: Runs unit tests.
- `omni doctor [--fix]`: Checks environment health and optionally tries to fix it.

## 6. Widget Reference (OmniUI 1.0)
### Core Widgets
- `Button`: A native-styled push button.
- `Slider`: A horizontal slider.
- `TextField`: Single-line text input.
- `Dial`: A rotary control, ideal for audio parameters.
- `CheckBox`: A boolean toggle.
- `ComboBox`: A dropdown selection menu.
- `ProgressBar`: Displays progress or metering.
- `JuceView`: A container for raw C++ JUCE Components.

### Charts & Data (OmniCharts 1.0)
- `LineChart`: High-performance 2D line plotter.
- `Oscilloscope`: Real-time waveform visualizer.

### Layouts (OmniLayout 1.0)
- `DockArea`: A flexible docking container.
- `SplitView`: A resizable split pane.

## 7. Audio & DSP (OmniAudio 1.0)
### Components
- `AudioProcessor`: Wraps a JUCE `AudioProcessor` graph.
- `AudioGraph`: Allows connecting multiple processors together.
- `AudioSource`: Plays sample files (supports looping).
- `Gain`: Simple volume control.
- `Filter`: LowPass/HighPass/BandPass filter.
- `Sequencer`: A simple MIDI step sequencer.
- `MidiHandler`: Provides slots to send NoteOn/Off events to the audio thread.

## 6. Widget Reference (OmniUI 1.0)
### Core Widgets
- `Button`: A native-styled push button.
- `Slider`: A horizontal slider.
- `TextField`: Single-line text input.
- `Dial`: A rotary control, ideal for audio parameters.
- `CheckBox`: A boolean toggle.
- `ComboBox`: A dropdown selection menu.
- `ProgressBar`: Displays progress or metering.
- `JuceView`: A container for raw C++ JUCE Components.

### Theming
Use the `Style` singleton to customize the application look.
```qml
import OmniUI 1.0
Style.primaryColor = "#ff0000"
```

## 7. Audio & MIDI (OmniAudio 1.0)
### Components
- `AudioProcessor`: Wraps a JUCE `AudioProcessor` graph.
- `AudioGraph`: Allows connecting multiple processors together.
- `AudioSource`: Plays sample files (supports looping).
- `Sequencer`: A simple MIDI step sequencer.
- `MidiHandler`: Provides slots to send NoteOn/Off events to the audio thread.

### Example: Audio Chain
```qml
AudioGraph {
    AudioSource { id: src; source: "drum.wav" }
    AudioProcessor { id: reverb; name: "Reverb" }

    Component.onCompleted: {
        addProcessor(src)
        addProcessor(reverb)
        connect(src, reverb)
    }
}
```
