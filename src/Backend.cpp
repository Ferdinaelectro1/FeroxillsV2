//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"
#include "core/FConstantes.h"
#include "core/FSettings.h"
#include "core/event/EventBus.h"
#include  "core/debug/debug.h"

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
    INFO("Launch app");
    signalGenerator = new SignalGenerator(this);
    _serialWorker = new SerialWorker(this);
    _sample_needed = FSettings::instance()->getSamplesNeeded();
    _displaySamples.resize(_sample_needed, 0.0);
    connect(signalGenerator,&SignalGenerator::samplesAvailable,this,&Backend::dataAvailable);
    connect(FSettings::instance(),&FSettings::onTimeDivChanged,this,&Backend::onTimeDivChanged);
    /*Réémission du signal issues du bus d'event par le backend , pour permettre de récupérer les paramètres du trigger depuis qml*/
    connect(EventBus::getInstance(),&EventBus::TriggerModeDisplayInvoked,this,&Backend::triggerModeDisplayInvoked);
    SignalParameter s_parameter;
    s_parameter.frequency = 150;
    s_parameter.voltage = 4;
    s_parameter.phase = 0;
    s_parameter.type = SignalType::SINUS;
    signalGenerator->setSignalParameter(s_parameter);
    signalGenerator->startAcquisition(100);
    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&Backend::onTimeOut); //timer d'affichage de chaque frame (on peut regler le fps ici)
    _timer->setInterval(50);
    _timer->start();
}

QVector<double> Backend::getDisplaySamples() const {
    return  _displaySamples;
}

double Backend::getMaxVoltage() const {
    return  _maxVoltage;
}

void Backend::setMaxVoltage(const QVector<double>& voltageSamples) {
    for (const double voltage :  voltageSamples) {
        if (voltage > _maxVoltage) {
            _maxVoltage = voltage;
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

void Backend::onTimeDivChanged() {
    _sample_needed = static_cast<unsigned long>((FSettings::instance()->getTimeDiv() * Feroxills::Constants::HORIZONTAL_DIVISIONS) / Feroxills::Constants::SAMPLING_PERIOD);
    FSettings::instance()->setSamplesNeeded(_sample_needed);
    _displaySamples.resize(_sample_needed, 0.0);
    const  double intervalOfAcquisition = Feroxills::Constants::SAMPLING_PERIOD * static_cast<double>(_sample_needed);
    qDebug() << "Samples need = " << _sample_needed << " et intervalOfAcquisition = " << intervalOfAcquisition*1000 << " et TimeDiv = "<<FSettings::instance()->getTimeDiv();
    signalGenerator->setAcquisitionInterval(static_cast<int>(intervalOfAcquisition*1000));
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
        _display_context.processDisplaySamples(&_samplesRingBuf,_displaySamples.data(),_sample_needed);
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
