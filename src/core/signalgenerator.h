#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QVector>
#include <QTimer>
#include <QDebug>

#define BUFFER_SIZE 512

enum class SignalType{
    SINUS,
    CARRE,
    TRIANGLE,
    CONTINU,
    RAMPE,
    RANDOM,
    PWM,
    ZERO
};

struct SignalParameter
{
    SignalType type = SignalType::SINUS;
    double voltage = 1.0;
    double phase = 0.0;
    double frequency = 10.0;
    double duty = 0.7;
};

class SignalGenerator : public QObject
{
    Q_OBJECT
public:
    explicit SignalGenerator(QObject *parent = nullptr);
    void start(int intervalle = 500);//demarrer l'envoie des signaux par intervalle
    void stop(); //arrêter l'envoie des signaux
    void setSignalParameter(const SignalParameter& type);
    void setSendingIntervalle(int intervalle);
    [[nodiscard]] SignalParameter getSignalParameter() const;
    void setDuty(double duty);
    void setVoltage(double voltage);
    void setFrequency(double frequency);

signals:
    void dataReady(const QVector<double>& );

private slots:
    void sendEchantillons();

private:
    QVector<double> m_echantillons;
    QVector<double> m_send_echantillons;
    QTimer* m_timer;
    SignalParameter m_current_signal_parameter;
    long m_t;
};

#endif // SIGNALGENERATOR_H
