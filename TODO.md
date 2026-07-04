# Short-Term Execution Goals (TODO)

## Phase 1 Parity Wrappers (Completed)
1. ~~**Build System Validation:**~~ Modularized `CMakeLists.txt` via `BobQSubmodules.cmake`.
2. ~~**Submodule Wrapper:**~~ Implemented `BobQJuceHost`.
3. ~~**Go Port Coverage:**~~ Generated `ANALYSIS.md`.
4. ~~**Documentation Sync:**~~ Generated `SUBMODULE_DASHBOARD.md`.
5. ~~**GTK Parity Research:**~~ Implemented `BobQBox` and `BobQGrid`.
6. ~~**Core Framework Renaming:**~~ Renamed bobui/qt to bqt globally across core source.

## Go Runtime Integration (Priority 1)
- [x] Add richer runtime integration for `Popup` and `Dialog`.
- [x] Add richer runtime integration for `Drawer`, `ToolTip`, and `Menu`.
- [x] Improve `ScrollView` and `ScrollBar` behavior.
- [x] Write tests for all above widgets inside `internal/ui/widgets/`.

## C++ Core Fixes (Priority 2)
- [x] Consolidate BQt CMake compatibility validation command and wire it into Go CI.
- [x] Complete the generic component registry (`BobQUppComponentRegistry`).

## Refactoring & Documentation
- [x] Outline U++ and JUCE bridging details explicitly inside `Manual.md` and the universal instruction suite.
- [x] Audit the BobQ C++ codebase to ensure cleanliness of the GTK implementation.
- [x] Continue porting `OmniAudioGraph` to pure Go.
- [x] Port components to Java and C# (Starting with `AudioGraph`)
- [x] Begin porting components to Rust (Starting with `AudioGraph`)
- [x] Port `OmniSynthesizer` to Rust, Java, and C#
- [x] Implement Go routines to handle BQt signal/slot emission seamlessly.
- [x] Map JUCE audio primitives into the `OmniAudioGraph` and `OmniSynthesizer` (Go port in progress).

- [x] Establish a unified Event Loop to drive BQt components seamlessly.
