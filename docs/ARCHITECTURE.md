# Architecture

This document explains the main architectural decisions and the runtime flow for Feroxills. It is written to help contributors understand why things are structured this way, not just what exists.

## Overview

Runtime flow (high level):
- Acquisition (provider, typically moved to a dedicated thread) → Backend (buffering, display timer) → QML UI (rendering and user controls).

Key components and their locations:
- Backend: `src/Backend.h` / `src/Backend.cpp` — display timer, circular buffer, connection point to `ProviderSourceController`.
- Provider system: `src/io/ISampleProvider.h`, `src/io/ProviderSettings.h`, `src/io/ProviderSourceController.h`.
- Providers: `SoftwareProvider` (`src/io/SoftwareProvider.*`) and `SerialProvider` (`src/io/SerialProvider.*`).
- QML UI and panels: `qml/Main.qml`, `qml/FerroRightPannel.qml`, `qml/SoftwareRightPanel.qml`, `qml/SerialRightPanel.qml`.
- Build and registration: `src/main.cpp` (QML type registration, `qInstallMessageHandler`).

## Provider system (`ISampleProvider` / `ProviderSettings`)

Contract (evidence: `src/io/ISampleProvider.h`):
- Public slots: `startAcquisition(const ProviderSettings*)`, `modifyAcquisitionSettings(const ProviderSettings*)`, `stopAcquisition()` — these forward to private virtual `doXxx` methods implemented by concrete providers.
- Signals: `samplesAvailable(const QVector<double>&)`, `providerStopped(ProviderState)`.

`ProviderSettings` is an abstract QObject-based settings container. Concrete settings implement:
- `Q_PROPERTY` per user-configurable field, a per-field changed signal, and an `anyFieldChanged()` signal.
- `clone()` returning a heap-allocated copy — required because `QObject` subclasses cannot be trivially copyable.

Why this design
- Decoupling acquisition from the rest of the app allows adding new input sources (software generator, serial, USB, file, network) without changing the backend or UI wiring.

## Orchestration (`ProviderSourceController`)

Responsibilities (see `src/io/ProviderSourceController.h`):
- Hold the current provider and its settings, expose reactive `Q_PROPERTY` values to QML (`currentProviderType`, `currentProviderSettings`).
- Create/switch providers and move them to a dedicated `QThread`.
- Emit `samplesAvailable` to the `Backend` which forwards to the UI pipeline.

Important implementation and pitfalls:
- Providers are moved to a dedicated `QThread` member — never call provider methods directly from the GUI thread; always use `QMetaObject::invokeMethod(..., Qt::QueuedConnection)` to queue operations into the provider thread.
- Capture provider pointers by value when posting lambdas across threads to avoid use-after-free if `_current_provider` changes before the lambda runs.
- Settings propagation uses `anyFieldChanged()` → `connectSettingsSignal()` → `setCurrentProviderSettings(...)` with a `clone()` used to safely transfer settings into the provider's thread.

## QML integration

- `FerroRightPannel.qml` contains a `Loader` whose source is chosen by `backend.sourceController.currentProviderType` and loads provider-specific panels such as `SoftwareRightPanel.qml` and `SerialRightPanel.qml`.
- Panels bind directly to `backend.sourceController.currentProviderSettings.<field>` for both reading and writing. This avoids extra plumbing through the `Backend` for settings changes.
- Enums and types are exposed to QML via `qmlRegisterUncreatableType` in `main.cpp` (e.g. `FDisplayMode`, `SignalType`, `ProviderType`).

## Backend responsibilities

- `Backend` manages the display timer, a ring buffer for incoming samples, and the display context (`DisplayContext`). It listens to `ProviderSourceController::samplesAvailable` and forwards frames to the view-model/UI.

## Logging

- A custom message handler (`debugMessageHandler`) is installed early in `main.cpp` via `qInstallMessageHandler(...)`. This formats `qDebug`/`qWarning`/`qCritical` and QML `console.log` messages.

## Known pitfalls and lessons
- Always marshal calls to provider objects to their thread via `QMetaObject::invokeMethod`.
- Do not destroy or reassign settings objects while they may be emitting signals — compare pointers before replacing.
- Avoid wide capture-by-reference (`[&]`) in lambdas that are executed asynchronously; capture only what is necessary, preferably by value.
