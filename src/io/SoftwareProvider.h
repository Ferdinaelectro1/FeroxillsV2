//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_SOFTWAREPROVIDER_H
#define FEROXILLS_SOFTWAREPROVIDER_H

#include <QVector>
#include <QTimer>
#include <QDebug>
#include <memory>
#include "ISampleProvider.h"
#include "ProviderSettings.h"
#include "src/core/SignalType.h"

#define BUFFER_SIZE 512

class SoftwareProvider;

class SoftwareProviderSettings final : public ProviderSettings {
    Q_OBJECT
    Q_PROPERTY(SignalType::Type type WRITE set_type READ get_type NOTIFY typeChanged)
    Q_PROPERTY(double voltage WRITE set_voltage READ get_voltage NOTIFY voltageChanged)
    Q_PROPERTY(double phase WRITE set_phase READ get_phase NOTIFY phaseChanged)
    Q_PROPERTY(double frequency WRITE set_frequency READ get_frequency NOTIFY frequencyChanged)
    Q_PROPERTY(double duty WRITE set_duty READ get_duty NOTIFY dutyChanged)
    Q_PROPERTY(double risingTime WRITE set_rising_time READ get_rising_time NOTIFY risingTimeChanged)
    Q_PROPERTY(int intervalMs WRITE set_interval_ms READ get_interval_ms NOTIFY intervalMsChanged)
public:
    [[nodiscard]] ProviderSettings *clone() const override {
        auto *new_copy_of_this = new SoftwareProviderSettings();
        new_copy_of_this->set_type(_type);
        new_copy_of_this->set_voltage(_voltage);
        new_copy_of_this->set_phase(_phase);
        new_copy_of_this->set_frequency(_frequency);
        new_copy_of_this->set_rising_time(_rising_time);
        new_copy_of_this->set_interval_ms(_interval_ms);
        new_copy_of_this->set_duty(_duty);
        return new_copy_of_this;
    }

    //getters
    [[nodiscard]] SignalType::Type get_type() const { return _type; }
    [[nodiscard]] double get_voltage() const { return _voltage; }
    [[nodiscard]] double get_frequency() const { return _frequency; }
    [[nodiscard]] double get_duty() const { return _duty; }
    [[nodiscard]] double get_rising_time() const { return _rising_time; }
    [[nodiscard]] int get_interval_ms() const { return _interval_ms; }
    [[nodiscard]] double get_phase() const { return _phase; }

    //setters
    void set_type(const SignalType::Type type) { _type = type; emit typeChanged(); }
    void set_voltage(const double voltage) { _voltage = voltage; emit voltageChanged(); }
    void set_frequency(const double frequency) { _frequency = frequency; emit frequencyChanged(); }
    void set_duty(const double duty) { _duty = duty; emit dutyChanged(); }
    void set_rising_time(const double rising_time) { _rising_time = rising_time; emit risingTimeChanged(); }
    void set_interval_ms(const int interval_ms) { _interval_ms = interval_ms; emit intervalMsChanged(); }
    void set_phase(const double phase) { _phase = phase; emit phaseChanged(); }

    signals:
    void typeChanged();
    void voltageChanged();
    void frequencyChanged();
    void dutyChanged();
    void risingTimeChanged();
    void intervalMsChanged();
    void phaseChanged();

private:
    SignalType::Type _type = SignalType::SINUS;
    double _voltage = 1.0;
    double _phase = 0.0;
    double _frequency = 10.0;
    double _duty = 0.7;
    double _rising_time = 0.5;
    int _interval_ms = 50;
};

class SoftwareProvider final : public ISampleProvider
{
    Q_OBJECT
public:
    explicit SoftwareProvider(QObject *parent = nullptr);

private:
    void doModifyAcquisitionSettings(const ProviderSettings *settings) override;
    void doStartAcquisition(const ProviderSettings *settings) override;
    void doStopAcquisition() override;

private slots:
    void send_Samples();

private:
    QVector<double> m_samples;
    QVector<double> m_samples_to_send;
    QTimer* m_timer;
    long m_t;
    std::unique_ptr<const SoftwareProviderSettings> m_settings;
};


#endif //FEROXILLS_SOFTWAREPROVIDER_H