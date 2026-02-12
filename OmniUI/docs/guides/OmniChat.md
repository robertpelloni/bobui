# OmniChat User Guide

OmniChat is an AI-powered assistant demonstrating local LLM integration and voice capabilities.

## Features
- **Local Inference**: Connects to Ollama via `OmniLLMClient`.
- **Voice Interaction**: Speech-to-Text and Text-to-Speech via `OmniVoice`.
- **Code Generation**: Displays generated code snippets in `OmniCodeEditor`.

## Architecture
- **Networking**: `OmniLLMClient` manages REST API calls to the local inference server.
- **Media**: `OmniVoice` handles audio input/output.
