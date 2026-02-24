//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"
#include "core/event/EventBus.h"

static void printSamplesParameter(const SamplesParameter& param) {
    constexpr double ech_freq = (1.0/44100.0);
    qDebug() << "*******Paramèters*******";
    qDebug() << "**Voltage Max = "<< param.voltage_max;
    qDebug() << "**Voltage Min = "<< param.voltage_min;
    qDebug() << "**First rising pos = "<<param._first_rising_pos;
    qDebug() << "**Second rising pos = "<<param._second_rising_pos;
    qDebug() << "**Period Samples = " << param.periodSamples;
    qDebug() << "**Period = "<<param.periodSamples * ech_freq;
    qDebug() << "**Frequence = "<<1/(param.periodSamples * ech_freq);
}

Backend::Backend(QObject *parent) : QObject(parent),_maxVoltage(0),_display_context(this,std::make_unique<ContinuMode>()) {
    signalGenerator = new SignalGenerator(this);
    connect(signalGenerator,&SignalGenerator::dataReady,this,&Backend::dataAvailable);
    /*Réémission du signal issues du bus d'event par le backend , pour permettre de récupérer les paramètres du trigger depuis qml*/
    connect(EventBus::getInstance(),&EventBus::TriggerModeDisplayInvoked,this,&Backend::triggerModeDisplayInvoked);
    SignalParameter s_parameter;
    s_parameter.frequency = 150;
    s_parameter.voltage = 4;
    s_parameter.phase = 0;
    s_parameter.type = SignalType::SINUS;
    signalGenerator->setSignalParameter(s_parameter);
    signalGenerator->start(100);
    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&Backend::onTimeOut); //timer d'affichage de chaque frame (on peut regler le fps ici)
    _timer->setInterval(50);
    _timer->start();
}

QVector<double> Backend::getDisplaySamples() const {
    QVector<double> vec(_displaySamples, _displaySamples + 512);
    return  vec;
}

double Backend::getMaxVoltage() const {
    return  _maxVoltage;
}

void Backend::setMaxVoltage(const QVector<double>& voltageSamples) {
    for (int i = 0; i< voltageSamples.size() ;i++) {
        if (voltageSamples[i] > _maxVoltage) {
            _maxVoltage = voltageSamples[i];
            emit MaxVoltageChanged();
        }
    }
}

bool Backend::get_run() const {
    return  _run;
}

void Backend::setRun(const bool run) {
    _run = run;
}

void Backend::dataAvailable(const QVector<double>& data) {
    if(data.size() != 512) {
        qWarning() << "Backend::dataAvailable(): data.size() != 512";
        exit(1);
    }

    _samples = data.toVector();

    for (const double ech : _samples)
        _samplesRingBuf.push(ech);

    setMaxVoltage(_samples);
}

void Backend::onTimeOut() {
    static bool analyse = false;
    static unsigned long count = 0;
    if (_run) {
        count++;
        if (!analyse && count > 200) {
            const QVector<double> analyseVec = _samplesRingBuf.getRecentWindows(512);
            const SamplesParameter param = _analyser.getSamplesParameter(analyseVec);
            if (param.isPeriodic)
              printSamplesParameter(param);
            else
                qWarning() << "Ce signal n'est pas un signal periodic";
            analyse = true;
        }
        //on envoie les données à afficher au système de traitement de l'affichage, pour décider de l'affichage
        //en utilisant le buffer circulaire
        _display_context.processDisplaySamples(&_samplesRingBuf,_displaySamples,512);
        emit SamplesChanged();
    }
}

double Backend::getDutyCycle() const {
    return signalGenerator->getSignalParameter().duty;
}

double Backend::getVoltage() const {
    return  signalGenerator->getSignalParameter().voltage;
}

double Backend::getFrequency() const {
    return signalGenerator->getSignalParameter().frequency;
}

void Backend::setDutyCycle(const double duty) const {
    signalGenerator->setDuty(duty);
}

void Backend::setVoltage(const double voltage) const {
    signalGenerator->setVoltage(voltage);
}

void Backend::setFrequency(const double frequency) const {
    signalGenerator->setFrequency(frequency);
}
