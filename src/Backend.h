//
// Created by ferdinand on 08/02/2026.
//

#ifndef FEROXILLS_BACKEND_H
#define FEROXILLS_BACKEND_H

#include "core/FBuffer.h"
#include <QTimer>

#include "ui/DisplayContext.h"
#include "core/analyser/SamplesAnalyser.h"
#include "core/FConstantes.h"
#include "io/SerialWorker.h"
#include  "io/ProviderSourceController.h"

class Backend final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double maxVoltage READ getMaxVoltage NOTIFY MaxVoltageChanged)
    Q_PROPERTY(DisplayContext * display_context READ getDisplayContext CONSTANT)
    Q_PROPERTY(bool run READ get_run WRITE setRun NOTIFY runChanged)
    Q_PROPERTY(double dutyCycle READ getDutyCycle WRITE setDutyCycle NOTIFY dutyCycleChanged)
    Q_PROPERTY(double voltage READ getVoltage WRITE setVoltage NOTIFY voltageChanged)
    Q_PROPERTY(double frequency READ getFrequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(ProviderSourceController * sourceController READ getProviderSourceController CONSTANT)
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend() override = default;
    [[nodiscard]] double getMaxVoltage() const;
    void setMaxVoltage(const QVector<double>& voltageSamples);
    [[nodiscard]] DisplayContext* getDisplayContext() { return &_display_context; }
    [[nodiscard]] bool get_run() const;
    [[nodiscard]] double getDutyCycle() const;
    [[nodiscard]] double getVoltage() const;
    [[nodiscard]] double getFrequency() const;
    [[nodiscard]] ProviderSourceController *getProviderSourceController() const ;
    void setDutyCycle(double duty) const;
    void setVoltage(double voltage) const;
    void setFrequency(double frequency) const;
    void setRun(bool run);

    signals:
        void displaySamplesReady(const QVector<double>& );
        void MaxVoltageChanged();
        void runChanged();
        void dutyCycleChanged();
        void voltageChanged();
        void frequencyChanged();
        void triggerModeDisplayInvoked(QObject *triggerDisplayMode);
        void suggestVoltPerDiv(float );

private:
    ProviderSourceController *_source_controller;
    QVector<double> _samples;
    FRingBuf<double,Feroxills::Constants::RING_BUFFER_SIZE> _samplesRingBuf;
    QVector<double> _displaySamples;
    double _maxVoltage;
    QTimer *_timer;
    DisplayContext _display_context;
    bool _run = true;
    SamplesAnalyser _analyser;
    unsigned long _sample_needed;
    SerialWorker *_serialWorker;

public slots:
    void onTimeOut();
    void dataAvailable(const QVector<double>& data);
    void onTimeDivChanged()  ;
};

#endif //FEROXILLS_BACKEND_H