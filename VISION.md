# BobQ Product Vision

## Long-Term Product Goal
BobQ aims to be the ultimate, universal cross-platform UI toolkit and runtime engine. The ultimate vision is an OS-agnostic execution environment capable of hot-swapping frontend rendering paradigms (Qt, Ultimate++, JUCE, GTK, DearImGui) transparently while backed by a heavily concurrent, memory-safe, pure Go kernel.

## Design Direction
The current paradigm is transitioning from complex C++ render-bridges (`QQuickPaintedItem` integrations) towards the "Go Transcendence." Future designs prioritize compiling directly via `gioui.org` abstractions and leveraging native Go subroutines for high-throughput DSP (`OmniAudioGraph`) and mesh networking without the heavy footprint of vendored C++ frameworks.
