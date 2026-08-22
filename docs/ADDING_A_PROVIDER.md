# Adding a new provider

Follow this checklist when adding a new acquisition provider. Reference implementation: `SerialProvider` (`src/io/SerialProvider.*`).

1) Settings
- [ ] Create `XxxSettings : public ProviderSettings`, `Q_OBJECT`.
- [ ] Add a `Q_PROPERTY` for every user-configurable field (READ/WRITE/NOTIFY).
- [ ] Each setter must emit its own `xChanged()` signal and `anyFieldChanged()`.
- [ ] Hardware-fixed characteristics (non-configurable) should remain private, without `Q_PROPERTY`.
- [ ] Implement `ProviderSettings::clone()` to return a heap-allocated copy of the concrete settings (field-by-field).

2) Provider implementation
- [ ] Create `XxxProvider : public ISampleProvider`, `Q_OBJECT`.
- [ ] Implement `doStartAcquisition(const ProviderSettings*)`, `doModifyAcquisitionSettings(const ProviderSettings*)`, `doStopAcquisition()`.
- [ ] In `doStartAcquisition` / `doModifyAcquisitionSettings`, `dynamic_cast` the incoming `ProviderSettings*` to `XxxSettings*` and `qWarning()` + return on failure.
- [ ] Store an owned `std::unique_ptr<const XxxSettings>` inside the provider for settings used by async handlers.
- [ ] Do not access `_settings` in the provider constructor — the settings object is supplied at start time.
- [ ] Any async handlers (e.g., readyRead) must be connected once (constructor) and must check `m_settings` is non-null inside the handler before using.

3) Factory
- [ ] Add creation logic to `ProviderFactory::createProvider` or the equivalent switch used by `ProviderSourceController`.

4) ProviderType enum and QML
- [ ] If adding a new provider type, add an entry to `ProviderType::Type` (see `src/io/ProviderType.h`).
- [ ] Ensure the QML registration in `main.cpp` still covers the type via `qmlRegisterUncreatableType`.

5) QML panel
- [ ] Create `XxxRightPanel.qml` mirroring the style of existing panels. Bind fields to `backend.sourceController.currentProviderSettings.<field>`.
- [ ] Add the panel to the `Loader` switch in `qml/FerroRightPannel.qml`.

6) Verification
- [ ] The provider compiles and instantiates without requiring changes to `ProviderSourceController` or `Backend` beyond the factory switch.
- [ ] Manual test: switch to the new provider via the UI and verify samples appear and settings are applied.
