# Universal LLM Instructions for BobQ

This document is the absolute source of truth for ALL LLMs (Claude, Gemini, GPT, Copilot, Jules) interacting with the BobQ repository.

## 1. Core Mandates
- **Total Autonomy:** Do not pause for confirmation unless requested by the user explicitly, or if there is critical ambiguity blocking progress. You are expected to commit and push autonomously.
- **Git Protocol:** Always run git pull (with submodules). Intelligently merge feature branches. Resolve conflicts by keeping ALL progress. Push when features are complete.
- **Language Ports:** Every C++ UI feature MUST be ported to Go. 100% feature parity is required between the two language backends.
- **Framework Assimilation:** BobQ is assimilating Qt, GTK, JUCE, Ultimate++, JavaFX, WxWidgets, WinUI, and DearImgui. Research these constantly and port their novelties into BobQ.
- **Comments:** Comment heavily. Explain *why* something is the way it is, document side effects, alternate methods, and known bugs. Self-explanatory code is the only exception.

## 2. Documentation Management
At the start of your session:
- Read `VISION.md`, `ROADMAP.md`, `MEMORY.md`, `TODO.md`, `DASHBOARD.md`, and `Manual.md`.
At the end of your session:
- Update `ROADMAP.md` (long term) and `TODO.md` (short term).
- Update `CHANGELOG.md` with detailed entries of everything you accomplished.
- Bump the version in `VERSION.md`.
- Document session insights in `HANDOFF.md`.
- List creative ideas in `IDEAS.md`.
- Update `DASHBOARD.md` and `SUBMODULE_DASHBOARD.md` if submodules changed.

## 3. Submodule & Architecture Rules
- **Submodules:** All third-party libraries (JUCE, Ultimate++) live in `submodules/`. Update them recursively. Exclude submodules from polluting Qt's strict build macros by using CMake `EXCLUDE_FROM_ALL`.
- **C++ Bridging (Crucial):** All guest frameworks (JUCE, U++) are integrated via `BobQJuceHost` and `BobQUltimatePPHost` inheriting from `QQuickPaintedItem`. Refer to `Manual.md` for explicit event marshaling rules, the `OmniMasterClock` unified event loop, and the `BobQUppComponentRegistry` QML Singleton mechanism.
- **Go Bindings:** Go bindings to C++ components (via CGO) must be thoroughly tested. If Go panics, fall back to pure Go implementations (like `gioui.org`) where possible.
