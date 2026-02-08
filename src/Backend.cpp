//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"

Backend::Backend(QObject *parent) : QObject(parent) {
    signalGenerator = new SignalGenerator(this);
    connect(signalGenerator,&SignalGenerator::dataReady,this,&Backend::dataAvailable);
    SignalParameter s_parameter;
    s_parameter.frequency = 150;
    s_parameter.voltage = 2.5;
    s_parameter.phase = 0;
    s_parameter.type = SignalType::SINUS;
    signalGenerator->setSignalParameter(s_parameter);
    signalGenerator->start();
}

QVector<double> Backend::getSamples() const {
    return  _samples;
}

void Backend::dataAvailable(const QVector<double>& data) {

    if(data.size() < 50) {
        qDebug() << "Taille de data reçu : "<<data.size();
        return;
    }
    qDebug() << "Taille de data reçu : "<<data.size();
    for(int i = 0; i< 50;i++)
    {
        qDebug() << "ech n° "<<i<<" = "<<data[i];
    }
    _samples = data.toVector();
    emit SamplesChanged();
}
