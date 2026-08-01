//
// Created by ferdinand on 05/03/2026.
//

#ifndef FEROXILLS_ISAMPLESREADER_H
#define FEROXILLS_ISAMPLESREADER_H
#include <qobject.h>
#include "ProviderSettings.h"
#include "ProviderType.h"

class ISampleProvider : public  QObject {
    Q_OBJECT

    public:
    explicit ISampleProvider(QObject *parent = nullptr) : QObject(parent) {};
    ~ISampleProvider() override = default;

public slots:
    void startAcquisition(const ProviderSettings* settings) {
        doStartAcquisition(settings);
    }
    void modifyAcquisitionSettings(const ProviderSettings* settings) {
        if (settings)
            doModifyAcquisitionSettings(settings);
    }
    void stopAcquisition() {
        doStopAcquisition();
    };

    signals:
    void providerStopped(ProviderState state);
    void samplesAvailable(const QVector<double>& );

private:
    virtual void doModifyAcquisitionSettings(const ProviderSettings* settings) {};
    virtual void doStartAcquisition(const ProviderSettings* settings) = 0;
    virtual void doStopAcquisition() = 0;
};

#endif //FEROXILLS_ISAMPLESREADER_H