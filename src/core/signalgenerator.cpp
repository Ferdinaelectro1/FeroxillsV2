#include "signalgenerator.h"
#include <QRandomGenerator>

SignalGenerator::SignalGenerator(QObject *parent)
    : ISampleProvider(parent)
{
    m_timer = new QTimer(this);
    m_echantillons.reserve(BUFFER_SIZE);
    m_echantillons.resize(BUFFER_SIZE);
    m_send_echantillons.reserve(BUFFER_SIZE);
    m_send_echantillons.resize(BUFFER_SIZE);
    connect(m_timer,&QTimer::timeout,this,&SignalGenerator::sendEchantillons);
    m_t = 0;
}

void SignalGenerator::doStartAcquisition(const int intervalle)
{
    m_timer->setInterval(intervalle);
    m_timer->start();
}

void SignalGenerator::doStopAcquisition()
{
    m_timer->stop();
}

void SignalGenerator::setSignalParameter(const SignalParameter& parameter)
{
    m_current_signal_parameter = parameter;
}

SignalParameter SignalGenerator::getSignalParameter() const
{
    return m_current_signal_parameter;
}

void SignalGenerator::doSetAcquisitionInterval(const int interval)
{
    m_timer->stop();
    m_timer->setInterval(interval);
    m_timer->start();
}

void SignalGenerator::sendEchantillons()
{
    // 1) swap: the filled buffer becomes sendable
    std::swap(m_echantillons, m_send_echantillons);

    // 2) send the previous buffer
    if (!m_send_echantillons.isEmpty()) {
        emit samplesAvailable(m_send_echantillons);
    }

    // 3) fill the new buffer
    m_echantillons.clear();
    m_echantillons.reserve(512);

    const double sampleRate = 44100.0;
    const double dt = 1.0 / sampleRate;

    for (int i = 0; i < 512; ++i) {
        const double t = m_t * dt;

        double value = 0.0;
        switch (m_current_signal_parameter.type) {
            case SignalType::SINUS:
                value =
                    m_current_signal_parameter.voltage *
                        std::sin(2.0 * M_PI *
                             m_current_signal_parameter.frequency * t +
                         m_current_signal_parameter.phase);
                break;
            case SignalType::AM:
                value = m_current_signal_parameter.voltage *
                        std::sin(2.0 * M_PI *
                             m_current_signal_parameter.frequency * t +
                         m_current_signal_parameter.phase) ;
                value =  value *
                        std::sin(2.0 * M_PI *
                             m_current_signal_parameter.frequency*30 * t +
                         m_current_signal_parameter.phase) ;
                break;
            case SignalType::FM:
                value = m_current_signal_parameter.voltage *
                        std::sin(2.0 * M_PI *
                             m_current_signal_parameter.frequency * t +
                         m_current_signal_parameter.phase);
                value =  m_current_signal_parameter.voltage *
                        std::sin(2.0 * M_PI *
                             value * t +
                         m_current_signal_parameter.phase) ;
                break;
            case SignalType::CONTINU:
                value = m_current_signal_parameter.voltage;
                break;
            case SignalType::CARRE:
                value = (std::sin(2.0 * M_PI *
                         m_current_signal_parameter.frequency * t +
                         m_current_signal_parameter.phase)  >= 0) ? m_current_signal_parameter.voltage : -m_current_signal_parameter.voltage;
                break;
            case SignalType::PWM:
            {
                const double phase = std::fmod(
                    m_current_signal_parameter.frequency * t +
                    m_current_signal_parameter.phase / (2.0 * M_PI),
                    1.0
                );

                value = (phase < m_current_signal_parameter.duty)
                        ? m_current_signal_parameter.voltage
                        : -m_current_signal_parameter.voltage;
            }
                break;
            case SignalType::TRIANGLE :
                value = 2 * m_current_signal_parameter.voltage / M_PI* std::asin(sin(2.0 * M_PI *
                         m_current_signal_parameter.frequency * t +
                         m_current_signal_parameter.phase));
                break;
            case SignalType::RAMPE:
                value = 2*m_current_signal_parameter.voltage * (
                    (t/(1.0/m_current_signal_parameter.frequency)) - floor(t/(1.0/m_current_signal_parameter.frequency) + 0.5)
                    );
                break;
            case SignalType::RANDOM: {
                constexpr double N = 1.0;
                value = m_current_signal_parameter.voltage* (QRandomGenerator::global()->generateDouble() * 2.0 - 1.0) * N;
                break;
            }
            case SignalType::RISING_PULSE:
                if (t >= m_current_signal_parameter.rising_time && t < m_current_signal_parameter.rising_time + 7*dt) {
                    value = m_current_signal_parameter.voltage;
                }
                else {
                    value = 0.0;
                }
                break;
            case SignalType::ZERO :
                value = 0.0;
                break;
            default:
                break;
        }

        m_echantillons.push_back(value);
        m_t++; // temps global avance
    }
}

void SignalGenerator::setDuty(const double duty) {
    m_current_signal_parameter.duty = duty;
}

void SignalGenerator::setVoltage(const double voltage) {
    m_current_signal_parameter.voltage = voltage;
}

void SignalGenerator::setFrequency(const double frequency) {
    m_current_signal_parameter.frequency = frequency;
}
