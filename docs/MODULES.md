# Modules and key files

This file lists the main modules, their responsibilities, and the central files to check when changing related functionality.

Core
- `src/core/FSettings.*` — application-scoped settings singleton exposed to QML.
- `src/core/FBuffer.*` — circular buffer utilities used by the backend.
- `src/core/analyser/SamplesAnalyser.*` — analysis utilities used by the backend for trigger detection and measurements.
- `src/core/event/EventBus.*` — lightweight event bus used for cross-component events.

IO (providers)
- `src/io/ISampleProvider.h` — provider interface; public slots forward to private virtual `doXxx` methods.
- `src/io/ProviderSettings.h` — abstract settings base; concrete settings implement `clone()`.
- `src/io/ProviderSourceController.*` — orchestrates provider creation, switching, threading, and settings propagation.
- `src/io/SoftwareProvider.*` — software signal generator provider (reference provider).
- `src/io/SerialProvider.*` — serial/UART provider using `QSerialPort`.
- `src/io/SerialPortUtils.h` — serial port helper utilities exposed as a QML singleton.

UI / Display
- `src/Backend.*` — display timer and bridge between providers and the view-model; owns `DisplayContext`.
- `src/ui/DisplayContext.*` and `src/ui/mode/*` — display mode implementations: Continu, Trigger, Auto.
- `src/ui/FViewModel.*` — view-model used to present samples to QML.

QML
- `qml/Main.qml` — application shell and registration points for panels.
- `qml/FerroRightPannel.qml` — right-side panel that loads provider-specific config panels via `Loader`.
- `qml/SoftwareRightPanel.qml`, `qml/SerialRightPanel.qml`, `qml/UsbRightPanel.qml` — provider-specific panels bound to `backend.sourceController.currentProviderSettings`.
- `qml/components/` — shared UI widgets (sliders, popups, etc.).

Build / registration
- `src/main.cpp` — QML type registration (enums and singletons) and `qInstallMessageHandler` installation.
- `CMakeLists.txt` — required Qt modules: Quick, QuickControls2, SerialPort; project targets and QML module list.
