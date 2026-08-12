//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERSOURCECONTROLLER_H
#define FEROXILLS_PROVIDERSOURCECONTROLLER_H

#include <QThread>
#include <memory>
#include <atomic>
#include <mutex>
#include "ISampleProvider.h"
#include  "ProviderSettings.h"
#include   "ProviderType.h"


class ProviderSourceController final : public QObject {
     Q_OBJECT
public:
     explicit  ProviderSourceController(ProviderType::Type  init_type, const ProviderSettings* init_settings, QObject *parent = nullptr);
     void switchTo(ProviderType::Type new_type, const ProviderSettings* new_settings);
     Q_INVOKABLE [[nodiscard]] ProviderType::Type getCurrentProviderType() const;
     Q_INVOKABLE [[nodiscard]] ProviderSettings *getCurrentProviderSettings() const;
     ~ProviderSourceController() override;

     signals:
     void samplesAvailable(const QVector<double>& );

private:
     ISampleProvider* initializeProviderSourceController(ProviderType::Type type,const ProviderSettings* settings) const;
     void setCurrentProviderSettings(const ProviderSettings* new_settings);
     void connectSettingsSignal();

     ISampleProvider *_current_provider;
     ProviderType::Type _pending_provider_source_type;
     std::unique_ptr<ProviderSettings> _pending_provider_settings;
     ProviderType::Type _current_provider_source_type;
     std::unique_ptr<ProviderSettings> _current_provider_settings;
     QThread* _current_provider_thread;
     bool _stop_in_progress;
     std::atomic<bool> _settingsUpdatePending{false};
     std::unique_ptr<const ProviderSettings> _pendingSettingsForApply;
     std::mutex _pendingSettingsMutex;
};


#endif //FEROXILLS_PROVIDERSOURCECONTROLLER_H