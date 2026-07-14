# OmniUI / BQt Framework

`bqt` is the **framework/kernel** project.

It provides the reusable foundation for:
- multi-device input and ownership
- UI widgets and layouts
- media / DSP primitives
- networking / synchronization primitives
- developer tooling overlays
- cross-platform runtime experiments

The **desktop shell / operating environment** belongs to the adjacent `bobfilez` project, which consumes `bqt`.

## Current State
This repository contains two major implementation tracks:
1. **C++ / BQt / OmniUI** in `OmniUI/`
2. **Experimental Go port** in `internal/` and `main.go`

Both tracks contain substantial work. Neither should be assumed complete without build verification.

## Design Direction
BQt aims to match or exceed practical strengths from:
- Qt 6 / Qt 7 direction
- JUCE
- JavaFX
- Dear ImGui / ImPlot

with special emphasis on:
- local multi-user interaction
- remote collaborative synchronization
- AI-aware context and automation
- high-performance desktop and WASM deployment

## Important Boundary
If you are looking for the actual OS shell, taskbar, launcher, or desktop product, that belongs in **`bobfilez`**, not in this repo.
