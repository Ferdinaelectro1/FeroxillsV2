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

#define BUFFER_SIZE 512

enum class SignalType{
    SINUS,
    CARRE,
    TRIANGLE,
    CONTINU,
    RAMPE,
    RANDOM,
    PWM,
    RISING_PULSE,
    AM,
    FM,
    ZERO
};

class SoftwareProviderSettings final : public  ProviderSettings {
public:
    SignalType type = SignalType::SINUS;
    double voltage = 1.0;
    double phase = 0.0;
    double frequency = 10.0;
    double duty = 0.7;
    double rising_time = 0.5;
    int interval_ms = 50;
    [[nodiscard]] ProviderSettings *clone() const override {
        return  new SoftwareProviderSettings(*this);
    }
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