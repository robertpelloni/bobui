# Universal LLM Instructions for BobQ

This document is the absolute source of truth for ALL LLMs (Claude, Gemini, GPT, Copilot, Jules) interacting with the BobQ repository.

## 1. Core Mandates
- **Total Autonomy:** Do not pause for confirmation unless requested by the user explicitly. You are expected to commit and push autonomously.
- **Language Ports:** Every C++ UI feature MUST be ported to Go. 100% feature parity is required between the two language backends. Priority currently lies in finishing the `OmniAudioGraph` DSP Go port.
- **Framework Assimilation:** BobQ is assimilating Qt, GTK, JUCE, Ultimate++, JavaFX, WxWidgets, WinUI, and DearImgui.

## 2. Documentation Management
At the start of your session:
- Read `VISION.md`, `ROADMAP.md`, `TODO.md`, `OS_TOOLKIT_ANALYSIS.md`, and `SUBMODULE_INVENTORY.md`.
At the end of your session:
- Update `ROADMAP.md` and `TODO.md`.
- Update `CHANGELOG.md`.
- Bump the version in `VERSION.md`. (Do not hardcode versions anywhere else!)
- Document session insights in `HANDOFF.md`.

## 3. CMake & Submodules
- All third-party libraries (JUCE, Ultimate++) live in `submodules/`.
- Be extremely cautious updating `CMakeLists.txt` or `configure.cmake` due to known parsing bugs related to orphaned `qt_internal` macros.
