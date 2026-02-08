# OmniUI Changelog

## [6.0.0] - 2025-02-23
### Added
- **Localization (OmniI18n)**: Implemented `OmniTranslator` for runtime language switching. Added `omni translate` CLI command.
- **3D Rendering (Omni3D)**: Implemented `OmniView3D`, `OmniMesh`, and `OmniLight` for Qt Quick 3D integration.
- **Testing (OmniTest)**: Implemented native QML unit testing with `OmniTestCase` and `OmniTestRunner`.
- **Deployment**: Added `omni deploy <platform>` command to the CLI.

## [5.0.0] - 2025-02-23
### Added
- **Plugin System**: `OmniPluginManager` and `OmniPluginInterface` for loading shared libraries.
- **Data Persistence**: `OmniDatabase` (SQLite) and `OmniSettings` (JSON/INI).
- **Advanced Graphics**: `OmniShaderEffect` and `OmniParticles`.
- **Pro Widgets**: `OmniCodeEditor` and `OmniCalendar`.
- **CLI**: Added `install` and `publish` commands.

## [4.0.0] - 2025-02-23
### Added
- **Charts**: `OmniLineChart` and `OmniOscilloscope`.
- **Layouts**: `OmniDockArea` and `OmniSplitView`.
- **Networking**: `OmniHttpClient` and `OmniWebSocket`.
- **DSP**: `OmniGain` and `OmniFilter`.
- **CLI**: Added `doctor --fix`.

## [3.0.0] - 2025-02-23
### Added
- **Core Widgets**: `OmniDial`, `OmniCheckBox`, `OmniComboBox`, `OmniProgressBar`.
- **Audio/MIDI**: `OmniAudioGraph`, `OmniAudioSource`, `OmniMidiSequencer`.
- **State/Style**: `OmniStyle` and `OmniState` singletons.
- **CLI**: Added `generate audio` and `test`.

## [2.0.0] - 2025-02-23
### Added
- **Widgets**: `OmniButton`, `OmniSlider`, `OmniTextField`.
- **Audio**: `JuceAudioProcessorWrapper` and `OmniMidiHandler`.
- **CLI**: Added `generate widget`.
- **Core**: Refactored `OmniApp` to use `QQmlApplicationEngine`.

## [1.0.0] - 2025-02-23
### Added
- Initial project structure (`core`, `cli`, `docs`).
- Basic Qt+JUCE bridge (`OmniApplication`, `JuceWidget`).
- CLI tool `omni`.
