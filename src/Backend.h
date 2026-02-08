//
// Created by ferdinand on 08/02/2026.
//

#ifndef FEROXILLS_BACKEND_H
#define FEROXILLS_BACKEND_H

#include "core/signalgenerator.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<double> samples READ getSamples NOTIFY SamplesChanged)
    Q_PROPERTY(double maxVoltage READ getMaxVoltage NOTIFY MaxVoltageChanged)
public:
    explicit Backend(QObject *parent = nullptr);
    QVector<double> getSamples() const;
    double getMaxVoltage() const;
    void setMaxVoltage(const QVector<double>& voltageSamples);

    signals:
        void SamplesChanged();
        void MaxVoltageChanged();

private:
    SignalGenerator *signalGenerator;
    QVector<double> _samples;
    double _maxVoltage;

public slots:
    void dataAvailable(const QVector<double>& data);
};

#endif //FEROXILLS_BACKEND_H