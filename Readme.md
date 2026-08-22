# Feroxills

Feroxills is a lightweight, developer-oriented oscilloscope application written in C++ and QML using Qt 6.5. It provides realtime signal acquisition, configurable display modes, and an extensible provider system for multiple input sources.

![Feroxills main window](docs/assets/screenshoot.png)

Features
- Software signal generator provider (reference implementation: `SoftwareProvider`).
- UART/Serial provider using `QSerialPort` (reference implementation: `SerialProvider`).
- Multiple display modes: Continuous (Roll), Trigger, and Auto (`Continu`, `Trigger`, `Auto`).
- Trigger detection and display plumbing via `SamplesAnalyser` and backend signals.
- Real-time user-adjustable signal settings exposed from QML via `backend.sourceController.currentProviderSettings`.
- QML-based UI with dynamic provider-specific settings panels (loaded by a `Loader` in `FerroRightPannel.qml`).
- Custom logging via `qInstallMessageHandler` (installed in `main.cpp`).

Build & run (Linux/macOS/Windows, Qt 6.5 required)

Prerequisites
- CMake >= 3.16
- Qt 6.5 with modules: `Quick`, `QuickControls2`, `SerialPort`

Build steps
```bash
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
# or to run directly from the build folder:
./build/appFeroxills
```

Where to look next
- Architecture and design details: `docs/ARCHITECTURE.md`
- How to add a new provider (checklist): `docs/ADDING_A_PROVIDER.md`

Project status
- Solo project by Ferdinand. Active development. This repository contains working implementations for the software generator and serial/UART provider; other provider types (USB) are enumerated but not implemented unless indicated in source files.
