//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"

Backend::Backend(QObject *parent) : QObject(parent),_maxVoltage(0) {
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
    connect(_timer,&QTimer::timeout,this,&Backend::onTimeOut);
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

void Backend::dataAvailable(const QVector<double>& data) {

    if(data.size() != 512) {
        qWarning() << "Backend::dataAvailable(): data.size() != 512";
        exit(1);
    }

    _samples = data.toVector();

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
    _samplesRingBuf.advanceRead(5);//on avance de 10 element
    _samplesRingBuf.getWindow(_displaySamples,512); //on met les nouveaux données dans le buffer d'affichage
    emit SamplesChanged();
}