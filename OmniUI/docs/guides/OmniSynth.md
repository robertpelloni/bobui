# OmniSynth User Guide

OmniSynth is a modular software synthesizer demonstrating the power of OmniUI's Audio Graph and Visual Node systems.

## Features
- **Visual Patching**: Connect oscillators, filters, and effects using the Node Editor.
- **Wavetable Synthesis**: Load custom wavetables or use built-in shapes.
- **Real-time Visualization**: Oscilloscope and Spectrogram for signal analysis.
- **MIDI Support**: Full keyboard control via `OmniMidiHandler`.

## Architecture
- **UI**: QML-based interface using `OmniNodes` for the graph and `OmniCharts` for analysis.
- **Audio Engine**: C++ backend (`OmniAudioGraph`) manages the DSP chain (`OmniWavetableSynth`, `OmniFilter`).
