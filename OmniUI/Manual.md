# OmniUI Manual

## Table of Contents
...
23. The Void (v39.0)
24. OmniGenesis (New in v40.0)
25. CLI

...

## 24. OmniGenesis (v40.0)
The final module. OmniGenesis allows the user to compress a decayed universe back into a singularity and ignite a Big Bounce, resetting the simulation parameters and starting anew.

### API Usage
```qml
import OmniGenesis 1.0

Text {
    text: "Current Cycle: " + SimulationLoop.cycleCount
}

Button {
    text: "Compress Void"
    onClicked: BigBounce.compressVoid()
}

Button {
    text: "Ignite Bang"
    enabled: BigBounce.criticalMass
    onClicked: BigBounce.igniteBang()
}
```

### Components
- **SimulationLoop**: The absolute meta-controller that tracks cycles (iterations of the universe).
- **SingularityCore**: The ultra-dense point of origin.
- **BigBounce**: The mechanism that triggers the universal reset.

### OmniSingularity Module Details (v41.0.0 Update)
The **OmniSingularity** module models Artificial General Intelligence (AGI) up to and including the technological singularity. In v41.0.0, the core C++ `OmniAGI` engine was fully implemented as an active simulation loop.

#### Key Enhancements:
*   **Active Cognitive Loop**: The `OmniAGI::tick()` loop runs at 10Hz, managing data ingestion and problem-solving compute cycles.
*   **Data Synthesis & Problem Solving**: Users can "feed" the AGI petabytes of raw data to increase its intelligence quotient. If assigned a complex problem, the AGI dynamically dedicates its processing power (PetaFLOPS) to reduce the problem complexity until a solution is emitted via the `solutionFound` signal.
*   **Runaway Self-Improvement**: Triggering `initiateRunawaySelfImprovement()` disables the safety limits. The AGI enters a recursive loop where intelligence breeds higher processing power, which breeds higher creativity, leading to exponential geometric growth.
*   **Singularity Threshold**: Once intelligence crosses an astronomical arbitrary limit (10,000,000+ IQ equivalent), the engine emits `singularityReached()`, classifying the AGI as a post-biological entity capable of answering any query instantly and ignoring containment commands.
*   **Comprehensive Control UI**: `OmniSingularityControl/assets/main.qml` was transformed into a neural monitoring dashboard.
    *   **Metrics Panel**: Displays dynamic, scaling readouts of IQ, Compute Power (shifting from PetaFLOPS to ExaFLOPS), and Non-Linear Synthesis (Creativity).
    *   **Live Output Log**: A scrolling, time-stamped log captures all system events, warnings, and solved answers directly from the C++ core.
    *   **Containment Controls**: Features an "Unshackle" button to initiate the singularity loop and a "Kill Switch" that only works if the singularity has not yet been reached.

### OmniGenesis Module Details (v41.0.0 Update)
The **OmniGenesis** module acts as the macroscopic framework for simulating universal life-cycles. In v41.0.0, the core C++ engine (`OmniSimulationLoop`) was implemented as a real-time thermodynamic state machine.

#### Key Enhancements:
*   **Thermodynamic Loop**: The `OmniSimulationLoop` runs an active timer simulating billions of years of cosmic expansion. It manages the interplay between raw Usable Energy (decreasing) and Universal Entropy (increasing).
*   **Cosmological Epochs**: As entropy naturally climbs, the universe automatically transitions through major cosmological eras: Inflationary Epoch -> Stelliferous Era -> Degenerate Era -> Black Hole Era -> Heat Death.
*   **Cycle Management**: The simulation tracks "Brahman Breaths", or cycles. A universe can naturally expire via Heat Death, or a user can trigger a "Big Crunch" which rapidly increases density and lowers entropy until it compresses back into a singularity, resetting the system for another Big Bang.
*   **Cosmic Simulator UI**: `OmniGenesisApp/assets/main.qml` was transformed into a "God-mode" universal control dashboard.
    *   **Parameters Panel**: Live progressing bars showing available Exergy (Work Energy) and Entropy.
    *   **Epoch Log**: A scrolling, time-stamped history of every cosmological milestone reached during the simulation, tracking the exact energy and entropy levels at the time of the event.
    *   **Interactive Controls**: Buttons allow the user to trigger the Big Bang from a singularity, force a Big Crunch to reset the cycle, or unnaturally accelerate thermodynamic decay (+dS). Every control has detailed tooltips explaining the astrophysics involved.

### OmniVoid Module Details (v41.0.0 Update)
The **OmniVoid** module represents interaction with absolute nothingness, specifically simulating a supermassive black hole/abyss environment. In v41.0.0, the `OmniAbyss` class became a live, destructive entity.

#### Key Enhancements:
*   **Abyssal Gravity & Evaporation**: The C++ core simulates mass continuously evaporating via Hawking Radiation. Conversely, if an observer "stares into the abyss," the gravity (proportional to its mass and inversely proportional to distance) actively pulls the user's proximity closer to 0.0 (the Event Horizon).
*   **Madness & Spaghettification**: Getting too close to the Event Horizon triggers `madnessInduced()` signals (causing UI shake effects). Crossing the threshold (0.0 AU) triggers a `spaghettificationEvent()`, instantly consuming the observer.
*   **Consumption Tracking**: You can sacrifice matter into the Void, which increases the Black Hole's mass and gravitational pull. A log tracks all consumed entities.
*   **Abyssal Monitor UI**: `OmniVoidWatcher/assets/main.qml` was transformed into a pitch-black monitoring system.
    *   **Telemetry Panel**: Tracks Event Horizon Mass, Hawking Radiation outgassing, and real-time Proximity to the horizon.
    *   **Visualizer**: A central, pulsing black circle grows in size relative to the black hole's mass and your proximity to it. An accretion disk effect flickers around the edge.
    *   **Controls**: You must click and hold the "Stare Into Abyss" button. If held too long, you are pulled past the event horizon, triggering a permanent "Death Screen" overlay. You can also type text to "Sacrifice" objects into the hole to see its mass increase.

### OmniMultiverse Module Details (v41.0.0 Update)
The **OmniMultiverse** module handles parallel realities functioning as isolated physics containers. In v41.0.0, the `OmniUniverseManager` was expanded to allow real creation, tracking, and destruction of these simulated universes.

#### Key Enhancements:
*   **Universe Array Management**: The core C++ engine tracks a `QMap` of `Universe` structs, each possessing a unique ID, Name, internal Energy Level, Physics Model (e.g., Standard Model, String Theory), and Membrane Stability.
*   **Incursions**: Universes can be forced to collide. Triggering `causeIncursion(source, target)` simulates a multiversal overlap. This bleeds energy between realities, increases global Cross-Reality Flux, and severely damages the target universe's Membrane Stability. If stability reaches 0%, the universe annihilates itself.
*   **Multiversal Observatory UI**: `OmniMultiverseExplorer/assets/main.qml` was transformed into a reality management matrix.
    *   **Meta-Stats Panel**: Tracks the number of active universes, total incurred collisions, and background flux radiation.
    *   **Creation Tools**: Users can spawn infinite new universes by specifying a name and selecting foundational physics laws from a dropdown.
    *   **Reality Roster**: A dynamic `ListView` shows all active universes, highlighting the one the user currently occupies. Each universe lists its name, physics model, base energy, and a live progress bar showing its Membrane Stability.
    *   **Interactive Controls**: Users can teleport between realities or intentionally trigger incursions to destroy neighboring universes. High-visibility red alert banners appear at the top of the UI when incursions are detected or realities are annihilated.

### OmniNano Module Details (v41.0.0 Update)
The **OmniNano** module provides a sub-nanometer scale physics simulation of atomic manipulation, mimicking a Scanning Tunneling Microscope (STM) molecular assembler.

#### Key Enhancements:
*   **Atomic State Management**: The C++ core `OmniLattice` tracks individual atoms as 3D coordinate structs, calculating real-time proximal collisions and basic covalent bonding counts.
*   **Probe Assembly**: The `OmniAssembler` simulates an STM probe. It must manually pick up atoms from a hopper, move to 3D Cartesian coordinates (in nanometers), and execute a placement function.
*   **Thermal Dynamics**: Atomic placement checks the simulated cryo-chamber temperature. If it exceeds 300K, placement fails due to simulated Brownian motion/thermal noise.
*   **NanoFactory UI**: `OmniNanoFactory/assets/main.qml` was rebuilt into a precision microscope interface.
    *   **Controls Panel**: Users can dial the cryo-temperature (with warnings if it gets too hot), see the exact atom loaded onto the tip, and input absolute X/Y/Z nanometer coordinates to move the probe.
    *   **Workspace `<Canvas>`**: A top-down 2D orthographic projection of the 3D atomic workspace. It draws gridlines, placed atoms (color-coded by element), and the active position of the STM crosshair tip.
    *   **Macros**: Includes a macro button to instruct the C++ engine to automatically loop through and print a complex 4x4x4 carbon diamondoid structure to prove the assembly engine works.

### OmniMind (BCI) Module Details (v41.0.0 Update)
The **OmniMind** module serves as the Brain-Computer Interface (BCI) processing backend. In v41.0.0, the mock generators were upgraded to a full neuro-simulation engine computing specific EEG bands.

#### Key Enhancements:
*   **Neural Simulation Engine**: The `OmniBCIProcessor` calculates real-time signal power for Delta (0.5-4Hz), Theta (4-8Hz), Alpha (8-12Hz), Beta (12-30Hz), and Gamma (>30Hz) brain waves.
*   **Live Raw EEG Generation**: The engine superimposes these frequency bands using trigonometric sine math to generate a realistic time-domain raw EEG electrical signal buffer, complete with physiological Brownian noise.
*   **Cognitive State Evaluator**: The `OmniBrainState` singleton constantly evaluates the ratios between these frequency bands to algorithmically determine the user's level of Focus, Relaxation, Stress, and overall Mental State (e.g., "Flow State", "Overwhelmed").
*   **Intention Translation**: The system watches for specific band spikes (e.g., high Gamma + high Beta) to translate raw brain activity into discrete system commands ("Execute", "Cancel").
*   **OmniMind Interface UI**: `OmniMind/assets/main.qml` was transformed into a neuro-diagnostic terminal.
    *   **Controls Panel**: Shows aggregated mental states (Focus, Stress, Relaxation) in dynamic progress bars. Provides buttons to synthetically stimulate specific mental states (like Deep Sleep or High Focus) which alters the underlying band targets over time.
    *   **Telemetry Panel**: Displays 5 vertical progress bars mapping real-time Spectral Power for every EEG band, along with a live `<Canvas>` that draws the raw superimposed voltage wave exactly as an oscilloscope would.
    *   **Intention Readout**: A prominent central screen that flashes translated textual commands whenever the system detects a specific thought pattern.

### OmniOmega Module Details (v41.0.0 Update)
The **OmniOmega** module functions as the overarching Universal Context Hub. In v41.0.0, the `OmniUniversalContext` class was implemented as an Inter-Process Communication (IPC) bus and central module registry.

#### Key Enhancements:
*   **Module Registry**: C++ now actively tracks all other OmniUI sub-modules, logging their internal IDs, operating states (Active, Suspended, Warning), and memory usage.
*   **Global Variables API**: OmniOmega acts as a singleton key-value store (`setGlobalVar`, `getGlobalVar`), allowing other modules to read shared state data (like "Security Level" or "User ID") without direct code coupling.
*   **System Event Bus**: The `broadcast(event, data)` function emits a global BobUI signal that any other QML or C++ class can listen to, facilitating decoupled interactions across the framework.
*   **Emergency Reboot**: Supports a complete wipe of the volatile state, emitting a "rebooting" signal to trigger shutdown sequences in connected modules.
*   **OmniOmega Nexus UI**: `OmniOmega/assets/main.qml` was rebuilt into a central system dashboard.
    *   **Registry Panel**: An interactive list displaying all tracked modules, color-coded by status, showing live memory usage. Users can click any module to assign it as the "Global Focus."
    *   **Global State Viewer**: A dynamic grid displaying all current key-value pairs stored in the context. Includes form fields to let users manually inject or overwrite global variables on the fly.
    *   **IPC Bus Tool**: A command-line style interface allowing users to manually construct and fire global broadcasts to test module interactions.
    *   **Reboot Overlay**: Pressing the metaphysical reboot button triggers a full-screen visual lockdown sequence while the C++ backend clears memory.

### OmniPortal Module Details (v41.0.0 Update)
The **OmniPortal** module simulates the physics, mechanics, and logistics of traversing a spacetime wormhole (Einstein-Rosen bridge).

#### Key Enhancements:
*   **Gate Dialing & Mechanics**: The C++ `OmniPortalGate` engine actively runs a 10Hz simulation loop. Puncturing spacetime requires an initial massive energy spin-up. Once open, the wormhole's structural integrity decays rapidly due to quantum fluctuations.
*   **Exotic Matter Injection**: To keep the throat of the wormhole propped open against its own gravity, users must manually inject negative-mass exotic matter to restore stability points.
*   **Probe Telemetry Simulation**: After a stable connection is established, users can dispatch a M.A.L.P. (Mobile Analytic Laboratory Probe). The C++ core simulates a delayed response, firing random probability logic to report if the target dimension is habitable, radioactive, or physically incompatible.
*   **Stargate Command UI**: `OmniPortalExplorer/assets/main.qml` was rebuilt as a full command center.
    *   **Gate Controls Panel**: Allows dialing specific coordinate strings, monitoring live structural integrity decay via progress bars, and seeing the massive simulated Terajoule energy draw.
    *   **Wormhole Visualizer**: A 2D render of the gateway ring. It physically rotates with an animation during the "Dialing" phase, lights up its chevrons when locked, and renders an animated blue event horizon when stable.
    *   **Telemetry Log**: A scrolling, text-based log where all C++ probe responses, error messages (like collapsing wormholes), and system statuses are output in real time.

### OmniSpirit Module Details (v41.0.0 Update)
The **OmniSpirit** module attempts to model metaphysical planes and astral entities within the OmniUI framework.

#### Key Enhancements:
*   **The Ethereal Plane Simulation**: `OmniEtherealPlane` acts as the active C++ state machine monitoring the "Veil" thickness between the physical and metaphysical worlds, and the "Collective Resonance" (an alignment scale from Abyssal Dark to Ascendant Light).
*   **Entity Manifestation**: The engine runs at 10Hz. If users interact recklessly with the plane (e.g., channeling too much raw energy or performing Dark Rituals), the Boundary Veil thickness drops. When it falls below 30%, spirits have a mathematical probability of manifesting in the physical world (spawning either Seraphs or Eldritch Horrors based on current Resonance).
*   **Veil Breach**: If the Veil drops to 0%, the realities merge, triggering a critical breach alarm.
*   **Astral Projection UI**: `OmniSpiritGuide/assets/main.qml` was rebuilt as a paranormal monitoring deck.
    *   **Metaphysical Metrics**: Features dual progress bars. One maps Collective Resonance (shifting from red to gold based on alignment). The other tracks Boundary Veil Thickness.
    *   **Ritual Controls**: Includes buttons to "Channel Positive Energy", "Perform Dark Ritual" (which aggressively damages the veil), or "Meditate" (which repairs it).
    *   **Manifestation Log**: A clean, scrolling interface showing all time-stamped paranormal events, including manifestations, alignment shifts, and critical veil breaches.

### OmniCreator Module Details (v41.0.0 Update)
The **OmniCreator** module provides an Integrated Development Environment (IDE) framework within OmniUI, simulating the build, linking, and compilation process.

#### Key Enhancements:
*   **Compiler Engine**: The `OmniCompiler` C++ class simulates a multi-threaded build process. It manages compilation phases ("Analyzing Dependencies", "Compiling AST", "Linking"), accurately driving a simulated progress bar toward 100%.
*   **Diagnostic Logging**: During the compilation phase, the engine dynamically generates log entries, categorizing them as INFO, WARN, ERROR, or SUCCESS. It keeps a rolling tally of total warnings and errors.
*   **Simulated Failures**: If the targeted project name is set exactly to "Fail", the engine simulates a fatal syntax error, halting the build and flagging the project as unsuccessful.
*   **OmniCreator IDE UI**: `OmniCreatorStudio/assets/main.qml` was transformed into a developer's compiler dashboard.
    *   **Build Config Panel**: Input fields to declare the target project name and a dropdown to select compilation targets (e.g., `aarch64`, `wasm32`, `omni-vPU`). Includes buttons to Clean, Build, or Cancel (send SIGTERM) to the running process.
    *   **Terminal Output**: A massive, auto-scrolling log view matching traditional IDE terminal outputs. Text is color-coded by severity (red for errors, yellow for warnings).
    *   **Progress Indicators**: Shows the exact build phase and a smoothly updating progress bar driven directly by the C++ engine's calculation ticks.

### OmniUniverse Module Details (v41.0.0 Update)
The **OmniUniverse** module provides a "God Mode" interface for fundamentally altering the laws of physics that govern all other modules.

#### Key Enhancements:
*   **Fundamental Constants Engine**: The C++ `OmniFundamentalConstants` singleton acts as the authoritative source for the Speed of Light (`c`), Gravitational Constant (`G`), Planck Constant (`h`), Boltzmann Constant (`k`), and the Fine Structure Constant (`α`).
*   **Anthropic Principle Evaluator**: A new `calculateConsequences()` C++ method runs a macro-physics check. If the Fine Structure Constant is altered too drastically, atoms fail to bind. If Gravity is tweaked too high, a rapid "Big Crunch" is simulated and the universe collapses. If physics break, the engine emits `physicsBroken()` signals.
*   **OmniGodMode UI**: `OmniGodMode/assets/main.qml` was transformed into a universal physics sandbox.
    *   **Constants Panel**: Users can manipulate precise scientific sliders to alter `c`, `G`, and `α` on the fly. The UI displays the scientific notation numbers in real-time.
    *   **Metaphysical Log**: A live scrolling terminal logs every user tweak to the physics engine. It also outputs the results of the "Evaluate Viability" check, throwing massive red errors if the user has mathematically destroyed the universe.
    *   **Safety Features**: Includes a "Reset to Standard Model" macro button to restore Earth-standard 21st-century physics.

### OmniGalactic Module Details (v41.0.0 Update)
The **OmniGalactic** module acts as a 4X Grand Strategy backend, simulating star system generation, hyperlane routing, and civilization expansion on a galactic scale.

#### Key Enhancements:
*   **Procedural Galaxy Generation**: The C++ `OmniGalacticMap` engine generates 100 star systems upon initialization using a cylindrical spiral-arm coordinate approximation algorithm. Stars are assigned realistic classifications (O, B, A, F, G, K, M, Black Hole, Neutron) with mass estimations.
*   **Fleet Transit Simulation**: The engine tracks a queue of dispatched colonization fleets. It calculates the exact 3D distance between systems in light-years. Fleets travel based on the civilization's current `technologyLevel` (higher tech = faster light-year per day traversal). Once distance reaches zero, the system is flagged as "controlled".
*   **Galactic Strategy UI**: `OmniCiv/assets/main.qml` was transformed into a command hub.
    *   **Empire Stats**: Shows population, government type, and tech level of your civilization.
    *   **Dynamic Map `<Canvas>`**: An interactive top-down 2D orthographic projection of the galaxy. It features a glowing galactic core, color-coded stars (based on class and ownership), and blue hyperlane connections linking controlled territories.
    *   **Interactive Selection**: Users can click directly on the canvas to select a star system. The UI fetches the C++ data for that specific star (Class, Mass, Distance from Core) and allows the user to dispatch a colonization fleet to it.
    *   **Transit Log**: A scrolling terminal showing the status of all active fleet movements and successful system securings.

### OmniTime Final Polish (Paradox Detector Extension) (v41.0.0 Update)
The **OmniTime** module was expanded to include causal paradox tracking.

#### Key Enhancements:
*   **Causality Physics**: The C++ `OmniParadoxDetector` engine actively monitors the `OmniTimeMachine` branch structure. If a user runs `analyzeCausality()`, the engine runs a probabilistic check for "Grandfather Paradoxes" or "Bootstrap Paradoxes" resulting from recent time jumps.
*   **Timeline Degredation**: Finding paradoxes increases the global probability of a reality collapse. The UI displays an alert level ("Stable Causality" up to "CRITICAL: Universe Ending Paradox").
*   **TVA Pruning**: Users can hit the "Purge Timeline Branch" button. This invokes `purgeTimeline()`, simulating the "pruning" of the divergent timeline, which instantly drops the paradox probability back to 0% and secures the causal loop.
*   **TimeTraveler UI Integration**: The QML UI was injected with a new "Causality Paradox Monitor" panel. It features a pulsing progress bar that turns bright red when the timeline is heavily fractured.

### OmniVR Module Details (v41.0.0 Update)
The **OmniVR** module acts as the "Full-Dive" or hardware interaction layer for Virtual/Augmented reality within OmniUI.

#### Key Enhancements:
*   **OmniXR Runtime Engine**: The C++ `OmniVRSystem` runs a 10Hz simulation loop replicating a Head-Mounted Display (HMD). It tracks 6-Degrees-of-Freedom (6DoF) Cartesian positional data, framerate targets (120Hz), and render latency.
*   **Vestibular Disconnect (Motion Sickness)**: A custom algorithm tracks the health of the user. If the runtime suffers from severe frame drops (<90Hz), high latency, or if the user utilizes "Smooth Locomotion" (moving the virtual camera without physical movement), a "sickness" meter rapidly fills.
*   **Emergency Disconnect**: If the motion sickness meter exceeds 95%, the system forces a session termination to protect the user.
*   **OmniVR Launcher UI**: `OmniVRLauncher/assets/main.qml` was transformed into a VR diagnostic dashboard.
    *   **Telemetry Panel**: Tracks HMD tracking status, live FPS (with color-coded warnings if it dips), and a highly-visible Vestibular Nausea progress bar.
    *   **Simulated Stress Tests**: Buttons allow developers to artificially inject tracking latency, force the GPU to thermal-throttle (dropping frames to 45Hz), or use smooth locomotion to purposefully test the nausea failsafe.
    *   **Visual Warning Overlay**: If sickness passes 80%, the screen begins tinting sickly-green, scaling in opacity until the emergency disconnect triggers.

### OmniLanguage Module Details (v41.0.0 Update)
The **OmniLanguage** module integrates a Large Language Model (LLM) emulation directly into the OmniUI ecosystem.

#### Key Enhancements:
*   **Transformer Emulation**: The C++ `OmniLinguisticModel` class manages a simulated chat history and streaming token generator.
*   **Tokenization & Context Tracking**: The module actively tracks the current Token Count context window (simulated 128k limit).
*   **Temperature / Chaos**: A dynamic Temperature property is exposed. If temperature exceeds 1.2, the generator intentionally outputs "hallucinated" syntax spaghetti and nonsense.
*   **OmniChat LLM Interface**: `OmniChat/assets/main.qml` was rebuilt into a full AI chat interface.
    *   **Configuration Panel**: Users can select the active model architecture, dial the temperature slider, and monitor their exact token context window via a progress bar. Includes buttons to clear history or halt active token streaming.
    *   **Chat Window**: A styled `ListView` separating System, User, and Assistant roles.
    *   **Live Token Streaming**: When generating, the UI renders the assistant's reply sequentially, mimicking realistic chunked LLM token streaming with variable latency.

### OmniHardware Module Details (v41.0.0 Update)
The **OmniHardware** module (embedded systems layer) simulates physical electronic interfaces within OmniUI.

#### Key Enhancements:
*   **GPIO Simulation Engine**: The C++ `OmniGPIOController` manages a 40-pin header array (mirroring a Raspberry Pi). It runs a 20Hz hardware simulation.
*   **Pin Architectures**: Hardcoded pins simulate 5V, 3.3V, and GND planes. Configurable pins can be set to "IN" (floating, with simulated random analog noise), "OUT" (driving 0.0 or 3.3V logic highs), or "PWM" (Pulse Width Modulation).
*   **PWM Emulation**: The engine calculates high-speed pin toggling based on user-defined duty cycles, providing a simulated "average voltage" output readout useful for driving motors or dimming LEDs.
*   **OmniHardware GPIO Monitor UI**: `OmniHardware/assets/main.qml` was transformed into an electronics lab bench.
    *   **Pin Grid Visualizer**: A dynamic, color-coded grid representing the 40 physical pins. Red for 5V, Orange for 3.3V, Black for GND, Purple for PWM. Digital "OUT" pins glow Green when driven HIGH. Tooltips over each pin show live logic states and simulated voltage values.
    *   **Logic Control Panel**: Provides text inputs and dropdowns to configure pin modes (IN/OUT/PWM). Users can manually toggle OUT pins HIGH or LOW, or attach a Slider to a PWM pin to sweep its duty cycle from 0% to 100% in real-time.

### OmniSynth Module Details (v41.0.0 Update)
The **OmniSynth** module is a Digital Signal Processing (DSP) engine simulating an audio synthesizer.

#### Key Enhancements:
*   **Wavetable Oscillator Engine**: The C++ `OmniWavetableSynth` class runs a 30 FPS visualizer buffer generator. It accurately mathematically constructs sound waves based on Pi and specific frequency (Hz) math for Sine, Square, Triangle, Saw, and Random Noise shapes.
*   **Phase and Amplitude Processing**: The C++ class properly calculates phase increments based on a simulated 44.1kHz sample rate, adjusting mathematically when the user alters the frequency or the absolute amplitude (volume).
*   **OmniSynth Oscilloscope UI**: `OmniSynth/assets/main.qml` was transformed into a professional audio engineer's dashboard.
    *   **Controls Panel**: Provides a dropdown to change the mathematical foundation of the wave, and smooth sliders to dynamically alter the frequency (20Hz to 2000Hz) and amplitude (0% to 100%).
    *   **Live Oscilloscope `<Canvas>`**: A large, dark-themed visualizer that graphs the 512-point audio buffer generated by the C++ backend. It features a glowing green trace overlaying a measurement grid, providing a highly responsive visualization of how frequency and amplitude alter the raw waveform.

### OmniDash Module Details (v41.0.0 Update)
The **OmniDash** module provides local system and hardware monitoring metrics for the host machine running the OmniUI framework.

#### Key Enhancements:
*   **System Diagnostics Engine**: The C++ `OmniSystem` class tracks simulated CPU, RAM, Disk I/O, Battery Level, and internal thermals using a 1Hz clock loop. It accurately fetches host machine parameters like OS type, host-name, and CPU Architecture using the `QSysInfo` BobUI class.
*   **Burn-in Testing**: The module allows developers to trigger a simulated 15-second stress test. This forcefully pegs CPU usage above 95%, maximizes RAM consumption, and drives the simulated CPU and GPU thermal sensors into critical (>90°C) territory, triggering UI alarms.
*   **Power Management**: Simulates battery discharging algorithms. If unplugged and running a stress test, the battery drains much faster.
*   **System Monitor UI**: `OmniDash/assets/main.qml` was rebuilt as a comprehensive hardware dashboard.
    *   **Resource Panel**: Displays progress bars for CPU, RAM, and Disk I/O. It dynamically reads out the actual Hostname and Architecture of the machine running the app.
    *   **Power Panel**: Features battery level and A/C connection status. Users can click "Toggle AC Power" to simulate a physical unplug event.
    *   **Thermal Array**: A beautifully animated `ListView` tracking multiple internal heat sensors (CPU Package, GPU Core, Motherboard). Temperature bars stretch and change color from blue -> orange -> red based on heat output, with borders glowing red if thermal limits are breached.

### OmniScribe Module Details (v41.0.0 Update)
The **OmniScribe** module provides a dynamic ECMAScript (JavaScript) execution environment allowing users to script logic inside the OmniUI ecosystem.

#### Key Enhancements:
*   **QJSEngine Integration**: The C++ `OmniScriptEngine` class wraps a full V8/QJSEngine. It accurately evaluates standard javascript text, handling syntax errors, parsing variables, and executing mathematical functions.
*   **C++ Object Bridging**: By injecting itself into the `globalObject()` as "Omni", scripts can call C++ methods like `Omni.print("text")` to pipe data out of the isolated Javascript sandbox back into the C++ UI framework.
*   **OmniScribe Execution Engine UI**: `OmniScribe/assets/main.qml` was transformed into a scripting IDE.
    *   **Script Editor Panel**: A multi-line raw text area initialized with a working Fibonacci algorithm using the custom `Omni.print()` C++ bridge. Users can write entire JS programs and press "Evaluate Script" to send them to the backend compiler.
    *   **Console Output**: A stylized terminal tracking standard inputs, custom `Omni.print()` outputs, and evaluation return values. It differentiates log types with colors (Grey for input, White for print, Gold for output returns, and Red with line numbers for Syntax Errors).
    *   **REPL Line**: A small Read-Eval-Print-Loop input bar at the bottom allows the user to fire off single-line Javascript commands (e.g. `2 + 2`) dynamically without needing to write a full script block.

### OmniStudio Module Details (v41.0.0 Update)
The **OmniStudio** module offers a Visual Logic Editor (Node-Based Workflow Designer) directly within the OmniUI framework, allowing for code-free logic assembly.

#### Key Enhancements:
*   **Custom QQuickPaintedItem Canvas**: The C++ `OmniNodeCanvas` class was implemented from scratch. It extends standard BobUI QML components by overriding native `paint()` methods to draw an infinite grid, floating logic blocks with drop shadows, and mathematically calculated bezier curves connecting the output and input ports of the nodes.
*   **Interactive Spatial Engine**: The C++ canvas handles complex mouse events seamlessly. Users can Left-Click and Drag to move nodes around the infinite space. Users can Right-Click and Drag to pan the entire camera view. Scroll Wheel events zoom in and out of the grid, adjusting scale and offset dynamically.
*   **OmniStudio Visual Editor UI**: `OmniStudio/assets/main.qml` was transformed into a sleek logic builder.
    *   **Node Palette**: Allows users to spawn new titled logic nodes into the workspace. Also provides a "Quick Connect" panel to link nodes together via index IDs.
    *   **Infinite Workspace**: The central UI area embeds the `OmniNodeCanvas`. It renders an interactive 2D physics space for node placement, complete with a mini-map readout showing current camera zoom level.

### OmniPhysics Module Details (v41.0.0 Update)
The **OmniPhysics** module is a custom 2D Impulse-Resolution physics engine embedded directly into the QML Scene Graph via C++.

#### Key Enhancements:
*   **Custom Physics World Component**: The C++ `OmniPhysicsWorld` manages a 60fps simulation loop. It applies Gravity (configurable per axis) and updates the velocities and positions of registered `OmniRigidBody` components using Euler integration.
*   **Impulse-Resolution Collision**: The engine uses Axis-Aligned Bounding Box (AABB) collision detection, calculating penetration vectors. It resolves overlapping boxes instantly and calculates resulting bounce vectors based on the specific Mass and Restitution (bounciness) of the colliding objects.
*   **Time Scaling**: The entire physics world can be slowed down or sped up via a `timeScale` multiplier, allowing for Matrix-style slow-motion debugging.
*   **OmniPhysicsLab UI**: `PhysicsLab/assets/main.qml` was transformed into an interactive 2D physics sandbox.
    *   **World Parameters**: Sliders to control X and Y gravity (simulating wind or inverted gravity) and Time Scale.
    *   **Interactive Spawners**: Buttons to dynamically spawn two types of objects into the scene:
        *   **Bouncy Box**: Low mass, extremely high restitution. Features a custom QML SequenceAnimation that squishes and stretches the box based on the `collided(impactSpeed)` signal fired from C++.
        *   **Heavy Crate**: 50x the mass of the bouncy box, very low restitution. Smashes the bouncy boxes out of the way.
    *   **The Sandbox**: A framed arena with static walls and a patterned floor that naturally bounds the physics objects.

### OmniWeb3 Module Details (v41.0.0 Update)
The **OmniWeb3** module simulates decentralized blockchain networks and cryptocurrency wallet interactions within OmniUI.

#### Key Enhancements:
*   **Wallet Connection & Balances**: The C++ `OmniWallet` singleton simulates connecting to Web3 providers (like MetaMask). It generates a random hex 0x address and seeds the user with a mock ETH balance on connection.
*   **Mempool Simulation Engine**: Transactions sent to the network are not confirmed instantly. They are pushed to a simulated mempool queue where a 5-second timer mimics block-mining. Transactions sit in a "Pending..." state until the timer completes, dropping them into "Confirmed" status and emitting a `transactionMined()` signal to alert the UI.
*   **Gas Physics**: Sending ETH or interacting with Smart Contracts (like `mintTokens()`) deducts gas from the user's balance based on a fluctuating `gasPrice` (Gwei) variable.
*   **OmniWeb3 Gateway UI**: `Web3Demo/assets/main.qml` was transformed into a sleek crypto terminal.
    *   **Wallet Profile**: Displays the connected address, formatted ETH balance (Ξ), and live network telemetry (Current Chain and fluctuating Gas price in Gwei).
    *   **Smart Contract Interaction**: Includes text fields to execute standard ETH transfers to arbitrary hex addresses, or interact directly with an OmniToken ERC-20 contract to mint synthetic tokens (which deducts gas fees).
    *   **Ledger Log**: A live `ListView` showing the status of all local transactions. Pending transactions are highlighted in yellow, flipping to green when the C++ mempool simulator mines their block.

### OmniNet Module Details (v41.0.0 Update)
The **OmniNet** module simulates a WebSocket connection, mimicking bi-directional JSON RPC payloads and network telemetry.

#### Key Enhancements:
*   **WebSocket Engine**: The C++ `OmniWebSocket` acts as both the client and a mocked remote server. Calling `open()` simulates a connection attempt with randomized latency. There is a 5% chance the remote host "refuses" the connection.
*   **Packet Jitter & Ping**: When connected, the engine actively modifies a ping variable to simulate network latency spikes, adding artificial delays when the mock server processes JSON payloads.
*   **JSON RPC Parsing**: If the client sends a valid JSON string (e.g. `{"type": "auth_request"}`), the C++ engine parses it and sets a `BOBUIimer` based on the current simulated latency. It then fires back an `ACK` response containing the requested type and a mock `200` status code. The mock server will also randomly broadcast `server_load` messages periodically.
*   **OmniNet Real-Time Packet Streamer UI**: `CollabSpace/assets/main.qml` was transformed into a network diagnostic tool.
    *   **Telemetry Panel**: Contains connection status, endpoint URI inputs, and a live progress-bar mapping the fluctuating simulated Ping in milliseconds.
    *   **JSON Injector**: A text area allowing users to write raw JSON objects and blast them down the open socket.
    *   **Packet Stream**: A scrolling `ListView` displaying all inbound (IN), outbound (OUT), and system (SYS/ERR) packet traffic, color-coded for debugging clarity.

### Omni3D Module Details (v41.0.0 Update)
The **Omni3D** module provides a custom-built software 3D rasterization engine implemented entirely in C++ over the QML `QQuickPaintedItem` canvas.

#### Key Enhancements:
*   **Custom 3D Rasterizer**: The C++ `OmniView3D` class manages a 60fps render loop. It constructs proper 4x4 View and Projection (Perspective) matrices based on an `OmniCamera` object.
*   **Vector Math & Projection**: It utilizes `QMatrix4x4` to translate, rotate, and scale `OmniMesh` objects into clip space, performs perspective division, and projects the resulting 3D vertices onto the 2D screen coordinates.
*   **Wireframe Rendering**: It features an active wireframe renderer that culls vertices behind the near-plane to avoid bizarre stretching artifacts when the camera moves through objects.
*   **Omni3D Rendering Engine UI**: `HelloOmni/assets/main.qml` was transformed into a 3D viewport.
    *   **Camera Controls**: Provides sliders to manipulate the X, Y (Elevation), and Z (Depth) coordinates of the `OmniCamera` in 3D space.
    *   **Live Viewport**: The right panel embeds the `OmniView3D` canvas, which renders a rotating cyan wireframe cube (`TestCube.qml` driven by QML Property Animations updating the C++ mesh rotation vectors).
    *   **Scene Management**: Users can dynamically click "Spawn Object" to inject new rotating 3D cubes into the scene, tracking the live active mesh and vertex counts on the UI.

### OmniML Module Details (v41.0.0 Update)
The **OmniML** module simulates the training and inference processes of a Deep Learning Neural Network.

#### Key Enhancements:
*   **Gradient Descent Simulator**: The C++ `OmniNeuralNetwork` class runs a 10Hz training loop. It mathematically simulates gradient descent, driving the "Loss" value down towards 0.0 while driving "Accuracy" up towards 100%. It features diminishing returns and injects random spikes/noise simulating the model getting stuck in local minima during stochastic batches.
*   **History Logger**: The class maintains a rolling `QVariantList` tracking the exact Loss and Accuracy parameters for the last 100 epochs, allowing real-time frontend graphing.
*   **Deep Learning Tensor Studio UI**: A dedicated `OmniMLTrainer/assets/main.qml` application was created.
    *   **Telemetry Panel**: Displays the current Epoch iteration, the precise loss calculation, and an animated progress bar showcasing the model's rising validation accuracy. Users can dynamically pause training or fully reset the model to initial random weights.
    *   **Convergence Graph `<Canvas>`**: A massive, interactive 2D graph that paints the real-time C++ tracking history. It plots Loss in red (scaling downwards) and Accuracy in blue (scaling upwards), mapping the classic neural net convergence pattern exactly as it happens.

### OmniNexus Module Details (v42.0.0 Update)
The **OmniNexus** module represents the final integration phase of the OmniUI project. It acts as the master desktop environment or "Command Bridge" that ties all 30 disparate scientific, cybernetic, and metaphysical simulations together into a single cohesive runtime.

#### Key Enhancements:
*   **The Master Clock (`OmniSimulationEngine`)**: In prior versions, modules ran on their own internal `BOBUIimer` instances at various frequencies (10Hz, 30Hz, 60Hz). In v42.0.0, the entire framework was unified. `OmniSimulationEngine` emits a `globalTick(double deltaTime)` signal. All modules (like `OmniPhysicsWorld` or `OmniKernel`) subscribe to this signal.
*   **Universal Time Dilation**: Because all physics, orbital mechanics, AGI processing, and thermodynamic decays are now bound to the `globalTick(dt)`, altering the `timeDilation` variable in the engine allows the user to universally speed up or slow down the entire simulated universe (from atomic lattice interactions in `OmniNano` up to `OmniGalactic` fleet movements) in perfect synchronization.
*   **Cross-Module IPC Routing**: Using the `OmniUniversalContext` (OmniOmega), events broadcasted by one module can trigger drastic reactions in another.
    *   **Example 1 (Kernel Panic)**: If a `KERNEL_PANIC` event fires, the IPC router intercepts it and automatically forces the `OmniAGI` module to halt processing (simulating a lack of compute resources) and drops the universal time dilation to `0.1x` to allow operators to debug the physical state of the universe in slow-motion.
    *   **Example 2 (Physics Broken)**: If the user alters the Fine Structure Constant in `OmniGodMode` to a degree that breaks physical binding laws, it fires `PHYSICS_BROKEN`. The router intercepts this, forces the `OmniMultiverse` manager to initiate a reality-destroying *Incursion*, and triggers the `OmniGenesis` module to slam the universe into a *Big Crunch*.
*   **OmniNexus Unified Command UI**: `OmniNexus/assets/main.qml` was built as the ultimate dashboard. It imports six major namespaces (`Omni.Universe`, `Omni.Omega`, `Omni.Kernel`, `Omni.Time`, `Omni.Multiverse`, `Omni.Singularity`) simultaneously into a beautiful 3x2 Grid Layout, allowing the operator to view the CPU load, AGI IQ growth, and Multiverse incursions side-by-side, while manipulating the Master Clock dilation slider at the bottom.

#### Omni Crisis Mode (v42.1.0 Update)
The `OmniNexus` module features a bright red **"INITIATE OMNI CRISIS PROTOCOL"** button at the bottom of the interface. This feature stress-tests the Inter-Process Communication (IPC) bus (`OmniUniversalContext`) and the Master Clock simultaneously.

Triggering the crisis fires a massive `OMNI_CRISIS` event over the IPC bus.
*   **OmniAGI Response**: Detects the signal, drops out of its recursive self-improvement loop, and transitions to an `EMERGENCY HALT` status, printing a catastrophic failure warning to its log.
*   **OmniEtherealPlane Response**: Automatically triggers three simulated "Dark Rituals", massively draining the Boundary Veil thickness, which triggers the high-probability spawning of Eldritch entities.
*   **OmniAbyss Response**: Ingests 50,000 solar masses of "Catastrophic Debris" instantly and activates the `stareInto()` function, aggressively pulling the simulated observer's horizon proximity toward 0.0 (Spaghettification).
*   **OmniTimeMachine / OmniSimulationEngine Response**: The central physics engine chokes its CPU tick rate down to 10Hz, and the Time Machine (if playing forward) violently reverses its progression speed to `-50.0x`.
