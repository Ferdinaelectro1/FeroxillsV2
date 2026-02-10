//
// Created by ferdinand on 08/02/2026.
//

#ifndef FEROXILLS_BACKEND_H
#define FEROXILLS_BACKEND_H

#include "core/signalgenerator.h"
#include "core/FBuffer.h"
#include <QTimer>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<double> samples READ getDisplaySamples NOTIFY SamplesChanged)
    Q_PROPERTY(double maxVoltage READ getMaxVoltage NOTIFY MaxVoltageChanged)
public:
    explicit Backend(QObject *parent = nullptr);
    QVector<double> getDisplaySamples() const;
    double getMaxVoltage() const;
    void setMaxVoltage(const QVector<double>& voltageSamples);

    signals:
        void SamplesChanged();
        void MaxVoltageChanged();

private:
    SignalGenerator *signalGenerator;
    QVector<double> _samples;
    FRingBuf<double,10000> _samplesRingBuf;
    double _displaySamples[512] = {0};
    double _maxVoltage;
    QTimer *_timer;

public slots:
    void onTimeOut();
    void dataAvailable(const QVector<double>& data);
};

#endif //FEROXILLS_BACKEND_H