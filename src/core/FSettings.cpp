//
// Created by ferdinand on 13/03/2026.
//

#include "FSettings.h"

FSettings::FSettings() : _localSettings("feroxills.org","Feroxills") {
    _timeDiv = _localSettings.value("timeDiv",0.001).toDouble();
    _ch1VoltDiv = _localSettings.value("ch1VoltDiv",4).toDouble();
    _sample_needed = static_cast<unsigned long>( _localSettings.value("samplesNeeded",441).toULongLong());
}

FSettings * FSettings::instance() {
    static FSettings instance;
    return &instance;
}

double FSettings::getTimeDiv() const {
    return _timeDiv;
}

double FSettings::getCh1VoltDiv() const {
    return  _ch1VoltDiv;
}

unsigned long FSettings::getSamplesNeeded() const {
    return _sample_needed;
}

void FSettings::setTimeDiv(const double timeDiv) {
    if (timeDiv != _timeDiv) {
        _timeDiv = timeDiv;
        emit onTimeDivChanged();
        _localSettings.setValue("timeDiv",timeDiv);
    }
}

void FSettings::setCh1VoltDiv(const double ch1VoltDiv) {
    if (ch1VoltDiv != _ch1VoltDiv) {
        _ch1VoltDiv = ch1VoltDiv;
        emit onCh1VoltDivChanged();
        _localSettings.setValue("ch1VoltDiv",ch1VoltDiv);
    }
}

void FSettings::setSamplesNeeded(const unsigned long samplesNeeded) {
    if (samplesNeeded != _sample_needed) {
        _sample_needed = samplesNeeded;
        _localSettings.setValue("samplesNeeded",static_cast<qulonglong>(samplesNeeded));
    }
}

//Temporaire
