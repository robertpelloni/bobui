# BQt Product Vision

## Long-Term Product Goal
BQt aims to be the ultimate, universal cross-platform UI toolkit and runtime engine. The ultimate vision is an OS-agnostic execution environment capable of hot-swapping frontend rendering paradigms (Qt, Ultimate++, JUCE, GTK, DearImGui) transparently while backed by a heavily concurrent, memory-safe, pure Go kernel.

## Design Direction
The current paradigm is transitioning from complex C++ render-bridges (`QQuickPaintedItem` integrations) towards the "Go Transcendence." Future designs prioritize compiling directly via `gioui.org` abstractions and leveraging native Go subroutines for high-throughput DSP (`OmniAudioGraph`) and mesh networking without the heavy footprint of vendored C++ frameworks.

## Scope & Boundaries
- **BQt (formerly BobUI):** The core framework kernel and unified event loop. This is the Go-first rendering and system services engine.
- **bobfilez:** The external OS shell that consumes BQt via the Shell Integration API layer. `bobfilez` exclusively manages user-facing web services, OS-level integrations, and external `.env` secrets. BQt is strictly the framework kernel and remains completely agnostic to these external service responsibilities to prevent scope drift.
