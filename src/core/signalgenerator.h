#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QVector>
#include <QTimer>
#include <QDebug>
#include "../io/ISampleProvider.h"

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
    ZERO
};

struct SignalParameter
{
    SignalType type = SignalType::SINUS;
    double voltage = 1.0;
    double phase = 0.0;
    double frequency = 10.0;
    double duty = 0.7;
    double rising_time = 0.5;
};

class SignalGenerator : public ISampleProvider
{

public:
    explicit SignalGenerator(QObject *parent = nullptr);
    void setSignalParameter(const SignalParameter& type);
    [[nodiscard]] SignalParameter getSignalParameter() const;
    void setDuty(double duty);
    void setVoltage(double voltage);
    void setFrequency(double frequency);

private slots:
    void sendEchantillons();

private:
    void doSetAcquisitionInterval(int interval) override;
    void doStartAcquisition(int intervalle) override;//demarrer l'envoie des signaux par intervalle
    void doStopAcquisition() override; //arrêter l'envoie des signaux
    QVector<double> m_echantillons;
    QVector<double> m_send_echantillons;
    QTimer* m_timer;
    SignalParameter m_current_signal_parameter;
    long m_t;
};

#endif // SIGNALGENERATOR_H
