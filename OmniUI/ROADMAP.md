# OmniUI Roadmap

## v8.0.0: Robustness & Depth (Current Focus)
The goal of this phase is to replace "stub" implementations with robust, production-grade logic, even if the underlying backend (JUCE) is simulated in this environment.

### 1. Audio Engine (`OmniAudio`)
- **Current**: `OmniAudioGraph` just logs connections.
- **Goal**: Implement a proper directed acyclic graph (DAG).
    - `OmniAudioConnection` class.
    - Cycle detection in `connect()`.
    - `disconnect()` method.
    - `OmniAudioMeter` for visual feedback.

### 2. Code Editor (`OmniWidgets`)
- **Current**: `QTextEdit` wrapper.
- **Goal**: Full IDE-lite features.
    - Syntax highlighting (`QSyntaxHighlighter`).
    - Line numbers margin.
    - Auto-indentation.

### 3. 3D Rendering (`Omni3D`)
- **Current**: Basic mesh loader.
- **Goal**: Camera control.
    - `OmniCamera` (Position, LookAt, FOV).
    - Input handling for orbit controls.

### 4. Hardware (`OmniHardware`)
- **Current**: Basic Serial wrapper.
- **Goal**: Configurable parameters.
    - Data bits, Parity, Stop bits.
    - Flow control.

## v17.0.0: Deep Implementation (Next)
- **3D**: Upgrade `OmniView3D` to manage a scene graph of `OmniMesh` items.
- **Physics**: Implement AABB collision detection and `collided` signals.
- **Web3**: Transaction history and state management.

## Future Phases
- **v18.0**: Advanced Networking (gRPC support).
- **v19.0**: Native Mobile Hardware (Camera/Sensors deep integration).
