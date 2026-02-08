# OmniUI Project Dashboard

## Overview
**Version:** 6.0.0
**Build Date:** 2025-02-23
**Status:** Stable Release

## Directory Structure

### `OmniUI/core` (The Framework)
The heart of OmniUI, containing all C++ source code and headers.
- `include/`: Public API headers.
    - `3d/`: 3D rendering classes (`OmniView3D`, `OmniMesh`).
    - `audio/`: DSP and Audio Graph classes.
    - `charts/`: Visualization widgets.
    - `data/`: Database and Settings.
    - `gfx/`: Shaders and Particles.
    - `i18n/`: Translation system.
    - `layout/`: Docking and Split views.
    - `net/`: HTTP and WebSocket clients.
    - `plugin/`: Plugin manager interface.
    - `test/`: Unit testing framework.
    - `widgets/`: UI controls (`OmniButton`, `OmniCodeEditor`, etc.).
- `src/`: Implementation files corresponding to `include/`.

### `OmniUI/cli` (The Tooling)
Node.js-based CLI for managing OmniUI projects.
- `bin/omni.js`: Main executable script.
- `templates/`: Project scaffolding templates.

### `OmniUI/docs` (Documentation)
- `Manual.md`: The primary user guide and API reference.

### `OmniUI/examples` (Demo Projects)
- `HelloOmni`: Basic "Hello World" app.
- `ComplexDemo`: Advanced workstation UI with charts and DSP.
- `PluginDemo`: Demonstrates plugins and data persistence.
- `Global3D`: Demonstrates 3D rendering and localization.

### `OmniUI/deps` (Dependencies)
- `juce/`: Mock headers for the JUCE library (bridge integration).

## Submodules
*Currently, OmniUI does not use external git submodules. All dependencies are either bundled or resolved via system package managers.*
