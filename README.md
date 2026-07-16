# BQt

BQt is an interoperable engine and framework kernel crossing C++, Go, Rust, Java, and C#. It provides a unified cross-framework architecture for UI and DSP.

## Note on Scope
BQt is the **framework kernel**, it is *not* a full OS shell. The OS shell capabilities are the separate scope of the `bobfilez` project.

## Go Port State
The Go port of BQt represents the primary runtime kernel, featuring:
*   **Unified Event Loop**: A central message queue (`internal/ui/event_loop.go`) that synchronizes and orchestrates tasks across sub-frameworks (e.g., GTK, JUCE, and U++).
*   **Native Signal/Slot System**: Executes slots asynchronously via goroutines to mirror Qt's queued connections, ensuring the event loop is never blocked.
*   **Audio Graph (`AudioGraph (OmniAudioGraph natively ported)`)**: Native Go parallel DSP processing simulating the JUCE `DeviceManager`. Includes nodes like `OmniGain` and `Synthesizer (OmniSynthesizer natively ported)` with 1:1 cross-language feature parity.

## Submodules
BQt incorporates powerful existing engines:
*   **JUCE**: Handled via `BobQJuceHost` and mapped into the Go `AudioGraph`.
*   **Ultimate++ (U++)**: Bridged through `BobQUppComponentRegistry` and `BobQUltimatePPHost`.
