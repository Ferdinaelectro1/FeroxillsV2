//
// Created by ferdinand on 8/1/26.
//

#include "SoftwareProvider.h"
#include <QRandomGenerator>

SoftwareProvider::SoftwareProvider(QObject *parent)
    : ISampleProvider(parent)
{
    m_timer = new QTimer(this);
    m_samples.reserve(BUFFER_SIZE);
    m_samples.resize(BUFFER_SIZE);
    m_samples_to_send.reserve(BUFFER_SIZE);
    m_samples_to_send.resize(BUFFER_SIZE);
    m_settings = nullptr;
    connect(m_timer,&QTimer::timeout,this,&SoftwareProvider::send_Samples);
    m_t = 0;
}

void SoftwareProvider::doStartAcquisition(const ProviderSettings *settings)
{
    const auto *softSettings = dynamic_cast<const SoftwareProviderSettings *>(settings);
    if (!softSettings) {
        qWarning() << Q_FUNC_INFO << "this ProviderSettings in parameter is not SoftWareSettings";
        return;
    }
    m_settings = std::make_unique<const SoftwareProviderSettings>(*softSettings);
    m_timer->setInterval(softSettings->interval_ms);
    m_timer->start();
}

void SoftwareProvider::doStopAcquisition()
{
    m_timer->stop();
    emit providerStopped(ProviderState::SUCCESS);
}

void SoftwareProvider::doModifyAcquisitionSettings(const ProviderSettings *settings)
{   const auto *softSettings = dynamic_cast<const SoftwareProviderSettings *>(settings);
    if (!softSettings) {
        qWarning() << Q_FUNC_INFO << "this ProviderSettings in parameter is not SoftWareSettings";
        return;
    }
    m_settings = std::make_unique<const SoftwareProviderSettings>(*softSettings);
    m_timer->stop();
    m_timer->setInterval(softSettings->interval_ms);
    m_timer->start();
}

void SoftwareProvider::send_Samples()
{
    // 1️⃣ swap : ce qui était rempli devient envoyable
    std::swap(m_samples, m_samples_to_send);

    // 2️⃣ envoyer le buffer précédent
    if (!m_samples_to_send.isEmpty()) {
        emit samplesAvailable(m_samples_to_send);
    }

    // 3️⃣ remplir le nouveau buffer
    m_samples.clear();
    m_samples.reserve(512);

    constexpr  double sampleRate = 44100.0;
    constexpr  double dt = 1.0 / sampleRate;

    for (int i = 0; i < 512; ++i) {
        const double t = m_t * dt;

        double value = 0.0;
        switch (m_settings->type) {
            case SignalType::SINUS:
                value =
                    m_settings->voltage *
                        std::sin(2.0 * M_PI *
                             m_settings->frequency * t +
                         m_settings->phase);
                break;
            case SignalType::AM:
                value = m_settings->voltage *
                        std::sin(2.0 * M_PI *
                             m_settings->frequency * t +
                         m_settings->phase) ;
                value =  value *
                        std::sin(2.0 * M_PI *
                             m_settings->frequency*30 * t +
                         m_settings->phase) ;
                break;
            case SignalType::FM:
                value = m_settings->voltage *
                        std::sin(2.0 * M_PI *
                             m_settings->frequency * t +
                         m_settings->phase);
                value =  m_settings->voltage *
                        std::sin(2.0 * M_PI *
                             value * t +
                         m_settings->phase) ;
                break;
            case SignalType::CONTINU:
                value = m_settings->voltage;
                break;
            case SignalType::CARRE:
                value = (std::sin(2.0 * M_PI *
                         m_settings->frequency * t +
                         m_settings->phase)  >= 0) ? m_settings->voltage : -m_settings->voltage;
                break;
            case SignalType::PWM:
            {
                const double phase = std::fmod(
                    m_settings->frequency * t +
                    m_settings->phase / (2.0 * M_PI),
                    1.0
                );

                value = (phase < m_settings->duty)
                        ? m_settings->voltage
                        : -m_settings->voltage;
            }
                break;
            case SignalType::TRIANGLE :
                value = 2 * m_settings->voltage / M_PI* std::asin(sin(2.0 * M_PI *
                         m_settings->frequency * t +
                         m_settings->phase));
                break;
            case SignalType::RAMPE:
                value = 2*m_settings->voltage * (
                    (t/(1.0/m_settings->frequency)) - floor(t/(1.0/m_settings->frequency) + 0.5)
                    );
                break;
            case SignalType::RANDOM: {
                constexpr double N = 1.0;
                value = m_settings->voltage* (QRandomGenerator::global()->generateDouble() * 2.0 - 1.0) * N;
                break;
            }
            case SignalType::RISING_PULSE:
                if (t >= m_settings->rising_time && t < m_settings->rising_time + 7*dt) {
                    value = m_settings->voltage;
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

        m_samples.push_back(value);
        m_t++; // global time increment
    }
}

