# BobQ Developer Manual

## 1. Overview
BobQ aims to be the ultimate, universal UI toolkit. To achieve this, it assimilates multiple distinct UI frameworks (Qt, Ultimate++, JUCE, and GTK concepts) into a single, cohesive engine, built primarily around the Qt Scenegraph (QML/C++) with a 1:1 functional Go port counterpart.

## 2. Multi-Framework Assimilation Architecture

### 2.1 The Render Bridge (`QQuickPaintedItem`)
Native C++ UI libraries like **JUCE** and **Ultimate++ (U++)** do not draw directly into the GPU pipeline the same way Qt's RHI (Rendering Hardware Interface) does. To integrate them natively into the BobQ QML Scenegraph, we employ a "render bridge" technique via `QQuickPaintedItem`.

*   **`BobQJuceHost`** and **`BobQUltimatePPHost`**: These C++ classes inherit from `QQuickPaintedItem`.
*   **Off-screen Buffer:** When the item needs to paint, we allocate an off-screen `QImage` or `QPainter` buffer.
*   **Delegation:** We instruct the underlying JUCE `Component` or U++ `Ctrl` to draw itself into this exact buffer.
*   **Compositing:** Qt then takes this software-rendered buffer and composites it as a texture into the hardware-accelerated QML scene.

### 2.2 Event Loop Unification (`OmniMasterClock`)
A primary challenge of intermixing UI frameworks is managing their distinct event loops. A typical application can only have one thread blocking on `select()` or `poll()`.

*   **The Master Loop:** BobQ forces Qt (`QGuiApplication`) to be the master event loop.
*   **The Pump:** The `OmniMasterClock` is a C++ singleton that runs a high-frequency `QTimer` (e.g., 60fps or 120fps).
*   **Siphoning Events:** Inside this timer tick, `OmniMasterClock` explicitly calls the guest framework event pumps (e.g., `juce::MessageManager::getInstance()->deliverBroadcastMessages()` and U++'s `Ctrl::ProcessEvents()`). This prevents deadlocks and ensures UI updates happen synchronously across all integrated frameworks on the main thread.

### 2.3 Event Marshaling
To interact with guest widgets embedded in QML:
*   The `BobQJuceHost` / `BobQUltimatePPHost` override Qt input handlers (`mousePressEvent`, `keyPressEvent`, `wheelEvent`).
*   When Qt receives a click, the host translates the `QMouseEvent` coordinates into the local coordinate space of the guest widget.
*   It then constructs the equivalent JUCE `MouseEvent` or U++ mouse event structure and injects it directly into the guest component's event handler, entirely bypassing the OS-level windowing system (X11/Wayland/Win32) for the guest framework.

### 2.4 Generic Component Registry (`BobQUppComponentRegistry`)
To fulfill the goal of truly unified declarative UI, developers must be able to summon guest widgets dynamically.

*   `BobQUppComponentRegistry` is registered as a QML Singleton (`qmlRegisterSingletonType`).
*   It maintains a factory map mapping string identifiers (like "Button" or "Slider") to their U++ (`Upp::Button`) or JUCE equivalents.
*   When requested via QML, the registry instantiates the guest widget, wraps it in the appropriate `QQuickPaintedItem` host, and returns it to the QML engine, making the distinction between a native Qt button and a U++ button completely transparent to the frontend developer.

## 3. Go Port Architecture
The Go port of BobQ maintains 1:1 feature parity. Where C++ uses `QQuickPaintedItem` to host foreign libraries, the Go port utilizes `gioui.org` to render pure Go implementations of these paradigms (such as GTK's `Box` packing or custom overlays). Future iterations of the Go port will use CGO to wrap the `BobQUppComponentRegistry` directly.
