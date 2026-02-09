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
- `omni install <pkg>`: Installs a plugin.
- `omni publish`: Builds a release package.
- `omni deploy <platform>`: Deploys to Android, iOS, WASM, etc.
- `omni translate <lang>`: Generates a translation template.
- `omni generate-node <name>`: Scaffolds a new visual node type.
- `omni generate-physics <name>`: Scaffolds a new physics entity.
- `omni login`: Authenticates with OmniCloud.
- `omni logout`: Signs out of OmniCloud.
- `omni repl`: Starts an interactive OmniScript shell.
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
- `CodeEditor`: Syntax-highlighting text editor.
- `Calendar`: Date picker widget.
- `JuceView`: A container for raw C++ JUCE Components.

### Data & Persistence (OmniData 1.0)
- `Settings`: Singleton for saving/loading app configuration.
- `Database`: Interface to SQLite storage.

### Graphics (OmniGfx 1.0)
- `ShaderEffect`: GLSL fragment shader container.
- `ParticleSystem`: GPU-accelerated particle emitter.

### 3D Rendering (Omni3D 1.0)
- `View3D`: High-performance 3D viewport.
- `Camera`: Controls position, look-at target, and FOV.
- `Mesh`: Loads 3D models (OBJ/GLTF).
- `Light`: Scene lighting source.

### Localization (OmniI18n 1.0)
- `Translator`: Singleton for switching languages at runtime.

### Desktop Integration (OmniDesktop 1.0)
- `SystemTray`: Native taskbar icon with menu.
- `Clipboard`: Copy/paste text.
- `DropArea`: File drag-and-drop support.

### Hardware I/O (OmniHardware 1.0)
- `SerialPort`: RS-232/USB UART communication.
- `Bluetooth`: BLE device discovery.

### Theming (OmniUI 2.0)
- `Theme`: Load JSON themes dynamically.

### Testing (OmniTest 1.0)
- `TestCase`: Unit test component.
- `TestRunner`: Executes registered test cases.

### Charts & Data (OmniCharts 1.0)
- `LineChart`: High-performance 2D line plotter.
- `Oscilloscope`: Real-time waveform visualizer.

### Layouts (OmniLayout 1.0)
- `DockArea`: A flexible docking container.
- `SplitView`: A resizable split pane.

### Networking (OmniNet 1.0)
- `HttpClient`: REST API client.
- `WebSocket`: Real-time bidirectional communication.
- `GraphQLClient`: Query and Mutate GraphQL endpoints.

### Cloud Services (OmniCloud 1.0)
- `Auth`: User authentication management.
- `Storage`: Cloud file upload/download.
- `PubSub`: Real-time messaging channels.

### Mobile Features (OmniMobile 1.0)
- `Sensors`: Access to accelerometer/gyroscope.
- `Haptics`: Vibration feedback.
- `Location`: GPS coordinates.

### Artificial Intelligence (OmniAI 1.0)
- `LLMClient`: Interface to local inference servers (e.g., Ollama).
- `Vision`: Image analysis.
- `Voice`: Speech-to-Text and Text-to-Speech.

### Scripting & Tooling (OmniScript 1.0)
- `ScriptEngine`: Run JavaScript dynamically.
- `FileWatcher`: Monitor filesystem changes (used for Hot Reload).

### Visual Nodes (OmniNodes 1.0)
- `NodeCanvas`: Infinite zoomable grid for node editing.
- `Node`: Base component for a visual node.
- `ConnectionLine`: Bezier curve connecting two points.

### Artificial Intelligence (OmniAI 1.0)
- `LLMClient`: Interface to local inference servers (e.g., Ollama).

### Animation (OmniAnim 1.0)
- `Track`: A timeline track containing keyframes.
- `Keyframe`: A time/value pair.

### Physics (OmniPhysics 1.0)
- `PhysicsWorld`: Simulation container.
- `RigidBody`: Dynamic object with mass and velocity.

### Shader Graph (OmniGfx 1.0)
- `ShaderNode`: Logic block for shader generation.
- `ShaderGenerator`: Compiles nodes to GLSL.

## 7. Audio & DSP (OmniAudio 1.0)
### Components
- `AudioProcessor`: Wraps a JUCE `AudioProcessor` graph.
- `AudioGraph`: Manages a graph of audio nodes with cycle detection.
- `AudioSource`: Plays sample files (supports looping).
- `AudioMeter`: Visualizes audio levels (RMS/Peak).
- `Sampler`: Multi-sample playback engine.
- `Wavetable`: Wavetable synthesizer (Sine, Saw, Square, Triangle).
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
