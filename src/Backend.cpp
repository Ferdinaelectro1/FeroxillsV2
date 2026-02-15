//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"

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
    SignalParameter s_parameter;
    s_parameter.frequency = 150;
    s_parameter.voltage = 4;
    s_parameter.phase = 0;
    s_parameter.type = SignalType::SINUS;
    signalGenerator->setSignalParameter(s_parameter);
    signalGenerator->start(1000);
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

    //qDebug() << "Données reçu" << p;
    if (_samplesRingBuf.freeSpace() ) {
        for (const double ech : _samples)
            _samplesRingBuf.push(ech);
    }
    else {
        qWarning() << "Plus d'espaces disponible pour pousser ce echantillons";
    }
    setMaxVoltage(_samples);
}

void Backend::onTimeOut() {
    static bool analyse = false;
    static unsigned long count = 0;
    if (_run) {
        _samplesRingBuf.advanceRead(5);//on avance de 5 element
        _samplesRingBuf.getWindow(_displaySamples,512); //on met les 512 nouveaux données issues du ringBuffer dans le buffer d'affichage
        count++;
        if (!analyse && count > 200) {
            const QVector<double> vec(_displaySamples, _displaySamples + 512);
            const SamplesParameter param = _analyser.getSamplesParameter(vec);
            printSamplesParameter(param);
            analyse = true;
        }
        _display_context.processDisplaySamples(_displaySamples,512);//on envoie les données à afficher au système de traitement de l'affichage, pour décider de l'affichage
        emit SamplesChanged();
    }
}