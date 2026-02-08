# OmniUI: The Definitive C++ UI Standard

## Overview
OmniUI is a next-generation UI framework that combines the industrial strength of **Qt 6** with the multimedia prowess of **JUCE**. It is designed to be the "Electron Killer" for C++, offering a developer experience (DX) akin to modern web development while maintaining native performance and compiling seamlessly to WebAssembly (WASM).

## Features
- **Qt + JUCE Fusion**: Use Qt for complex layouts, styling, and accessibility, while embedding JUCE components for high-performance audio/visualizers.
- **TypeScript First**: Write your UI logic and business logic in TypeScript. OmniUI transpiles this to optimized QML/C++ bindings.
- **WebAssembly Native**: Zero-config WASM builds. Your desktop app is your web app.
- **Developer Experience**: A CLI tool `omni` that handles scaffolding, building, and hot-reloading.

## Getting Started
See the [User Manual](docs/Manual.md) for full documentation.

### Quick Start
```bash
npm install -g omni-ui-cli
omni init my-app
cd my-app
omni dev
```
