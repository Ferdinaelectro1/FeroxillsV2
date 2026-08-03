//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERSOURCECONTROLLER_H
#define FEROXILLS_PROVIDERSOURCECONTROLLER_H

#include <QThread>
#include <memory>
#include "ISampleProvider.h"
#include  "ProviderSettings.h"
#include   "ProviderType.h"


class ProviderSourceController final : public QObject {
     Q_OBJECT
public:
     explicit  ProviderSourceController(ProviderType  init_type, const ProviderSettings* init_settings, QObject *parent = nullptr);
     void switchTo(ProviderType new_type, const ProviderSettings* new_settings);
     [[nodiscard]] ProviderType getCurrentProviderType() const;
     [[nodiscard]] ProviderSettings *getCurrentProviderSettings() const;
     void setCurrentProviderSettings(const ProviderSettings* new_settings);
     ~ProviderSourceController() override;

     signals:
     void samplesAvailable(const QVector<double>& );

private:
     ISampleProvider* initializeProviderSourceController(ProviderType type,const ProviderSettings* settings) const;

     ISampleProvider *_current_provider;
     ProviderType _pending_provider_source_type;
     std::unique_ptr<ProviderSettings> _pending_provider_settings;
     ProviderType _current_provider_source_type;
     std::unique_ptr<ProviderSettings> _current_provider_settings;
     QThread* _current_provider_thread;
     bool _stop_in_progress;
};


#endif //FEROXILLS_PROVIDERSOURCECONTROLLER_H