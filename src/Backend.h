//
// Created by ferdinand on 08/02/2026.
//

#ifndef FEROXILLS_BACKEND_H
#define FEROXILLS_BACKEND_H

#include "core/signalgenerator.h"
#include "core/FBuffer.h"
#include <QTimer>

#include "ui/DisplayContext.h"

class Backend final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<double> samples READ getDisplaySamples NOTIFY SamplesChanged)
    Q_PROPERTY(double maxVoltage READ getMaxVoltage NOTIFY MaxVoltageChanged)
    Q_PROPERTY(DisplayContext * display_context READ getDisplayContext CONSTANT)
    Q_PROPERTY(bool run READ get_run WRITE setRun)
public:
    explicit Backend(QObject *parent = nullptr);
    [[nodiscard]] QVector<double> getDisplaySamples() const;
    [[nodiscard]] double getMaxVoltage() const;
    void setMaxVoltage(const QVector<double>& voltageSamples);
    [[nodiscard]] DisplayContext* getDisplayContext() { return &_display_context; }
    [[nodiscard]] bool get_run() const;
    void setRun(bool run);


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
    DisplayContext _display_context;
    bool _run = true;

public slots:
    void onTimeOut();
    void dataAvailable(const QVector<double>& data);
};

#endif //FEROXILLS_BACKEND_H