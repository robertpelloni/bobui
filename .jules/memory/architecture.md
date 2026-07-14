# BobQ / BQt Project Memory & Architecture

## Core Vision & Goal
The framework is undergoing a massive transformation and assimilation process across multiple layers (C++, Go, Rust) and submodules (JUCE, Ultimate++, GTK, Qt). The goal is total interoperability and a unified Event Loop.

### Current Task Progress
- **Framework Rename:** Replaced all instances of `bobui`, `bobqt`, and `qt` with `bqt` across `internal/`, `BobQ/`, and `OmniUI/` directories.
- **Go Porting:** Actively porting C++ logic to Go. We've just completed adding `Start()` and `Stop()` methods to `internal/audio/audio_graph.go` to achieve parity with the `OmniAudioGraph` C++ class.
- **Continuous Documentation:** Strict rules mandate that `CHANGELOG.md`, `ROADMAP.md`, `TODO.md`, `HANDOFF.md`, and `VERSION.md` are kept strictly in sync with code evolution.

## Architecture Decisions
- The project relies heavily on `CMake` for C++ components and standard `go build` tools for the backend UI/DSP logic.
- We maintain a strict boundary when renaming variables to avoid breaking submodules.
- The Go backend logic requires 100% feature parity with C++ components (like `OmniGain` and `OmniSynthesizer`).

I will now commit the internal Go renames and finish the task properly by making sure the changes pass tests and code review.