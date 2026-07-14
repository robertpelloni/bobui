# BobQ Deployment and Setup Instructions

## 1. Environment Requirements
- **Go:** >= 1.22
- **CMake:** >= 3.16
- **C++ Toolchain:** GCC 13+ or equivalent MSVC/Clang environment.
- **System Libraries (Linux):** `libvulkan-dev`, `libegl1-mesa-dev`, `libxkbcommon-x11-dev`, `libwayland-dev` required for Go Gio.org compilation.

## 2. Secrets & API Keys
No API keys should ever be hard-coded into the repository.
- If adding external web services, utilize `.env` files.
- Copy `.env.example` to `.env` locally before running deployment pipelines.

## 3. Go Compilation (Preferred)
The easiest way to execute the engine is using the native Go port, which bypasses legacy Qt/CMake evaluation bugs.
```bash
go mod tidy
go build -buildvcs=false .
./bobui
```

## 4. Native C++ Compilation (Legacy)
Due to aggressive macro refactoring during the BobUI -> BobQ rename, the CMake configuration script currently has known macro evaluation errors (e.g. `RE_shared`). Native C++ builds should only be attempted if explicitly maintaining the `BobQJuceHost` or `BobQUltimatePPHost` bridges.
```bash
mkdir build && cd build
cmake -GNinja ..
ninja
```
