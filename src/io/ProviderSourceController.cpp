//
// Created by ferdinand on 8/1/26.
//

#include "ProviderSourceController.h"
#include "Factory.h"

ProviderSourceController::ProviderSourceController(const ProviderType::Type init_type, const ProviderSettings* init_settings, QObject *parent) : QObject(parent) {
    _current_provider_thread = new QThread(this);
    _current_provider_thread->start();
    _current_provider_source_type = init_type;
    _current_provider_settings = std::unique_ptr<ProviderSettings>(init_settings->clone());
    connectSettingsSignal();
    _pending_provider_source_type = init_type;
    _current_provider = initializeProviderSourceController(init_type, _current_provider_settings.get());
    qRegisterMetaType<ProviderState>("ProviderState");
    if (_current_provider) connect(_current_provider,&ISampleProvider::samplesAvailable,this,&ProviderSourceController::samplesAvailable);
    _stop_in_progress = false;
}

ProviderSourceController::~ProviderSourceController() {
    if (_current_provider) {
        ISampleProvider* provider_to_stop =  _current_provider;
        QMetaObject::invokeMethod(_current_provider,[provider_to_stop]() {
            provider_to_stop->stopAcquisition();
        },Qt::QueuedConnection);
        _current_provider->deleteLater();
    }
    _current_provider_thread->quit();
    _current_provider_thread->wait();
}


void ProviderSourceController::switchTo(const ProviderType::Type new_type, const ProviderSettings* new_settings) {
    if (!new_settings || _stop_in_progress) return;
    if (!_current_provider) {
        auto temp_settings = std::unique_ptr<ProviderSettings>(new_settings->clone());
        _current_provider = initializeProviderSourceController(new_type,temp_settings.get());
        if (!_current_provider) return;
        _current_provider_settings = std::move(temp_settings);
        connectSettingsSignal();
        _current_provider_source_type = new_type;
    } else {
        _pending_provider_source_type = new_type;
        _pending_provider_settings = std::unique_ptr<ProviderSettings>(new_settings->clone());
        connect(_current_provider,&ISampleProvider::providerStopped,this,[this](const ProviderState state) {
            if (state == ProviderState::SUCCESS) {
                if (_current_provider) _current_provider->deleteLater();
                _current_provider = initializeProviderSourceController(_pending_provider_source_type,_pending_provider_settings.get());
                if (!_current_provider) return;
                _current_provider_source_type = _pending_provider_source_type;
                _current_provider_settings = std::move(_pending_provider_settings);
                connectSettingsSignal();
            }
            _stop_in_progress = false;
        });
        _stop_in_progress = true;
        if (_current_provider) _current_provider->stopAcquisition();
    }
}

ProviderType::Type ProviderSourceController::getCurrentProviderType() const {
    return  _current_provider_source_type;
}

ProviderSettings * ProviderSourceController::getCurrentProviderSettings() const {
    return _current_provider_settings.get();
}

void ProviderSourceController::setCurrentProviderSettings(const ProviderSettings *new_settings) {
    if (!_current_provider || !new_settings) return;
    {
        std::lock_guard<std::mutex> lock(_pendingSettingsMutex);
        _pendingSettingsForApply = std::unique_ptr<const ProviderSettings>(new_settings->clone());
    }
    if (_settingsUpdatePending.exchange(true,std::memory_order_release)) return;

    ISampleProvider* provider = _current_provider;
    QMetaObject::invokeMethod(provider, [this,provider]() mutable {
        _settingsUpdatePending.store(false);
        std::unique_ptr<const ProviderSettings> settings;
        {
            std::lock_guard<std::mutex> lock(_pendingSettingsMutex);
            settings = std::move(_pendingSettingsForApply);
        }
        if (settings && provider)
            provider->modifyAcquisitionSettings(settings.get());
    }, Qt::QueuedConnection);
    _current_provider_settings = std::unique_ptr<ProviderSettings>(new_settings->clone());
    connectSettingsSignal();
}

ISampleProvider* ProviderSourceController::initializeProviderSourceController(const ProviderType::Type type, const ProviderSettings* settings) const {
    ISampleProvider *new_provider = ProviderFactory::createProvider(type);
    if (!new_provider) return nullptr;
    auto settings_copy = std::unique_ptr<const ProviderSettings>(settings->clone());
    new_provider->moveToThread(_current_provider_thread);
    QMetaObject::invokeMethod(new_provider,[new_provider,settings_copy = std::move(settings_copy)]() mutable {
        new_provider->startAcquisition(settings_copy.get());
    },Qt::QueuedConnection);
    return new_provider;
}

void ProviderSourceController::connectSettingsSignal() {
//We connect this new signal because settings changed , so new settings should have, his own anyFieldChanged signal
  if (_current_provider_settings) connect(_current_provider_settings.get(),&ProviderSettings::anyFieldChanged,this,[this]() {
      this->setCurrentProviderSettings(_current_provider_settings.get());});
}