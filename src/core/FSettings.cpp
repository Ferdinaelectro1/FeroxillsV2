//
// Created by ferdinand on 13/03/2026.
//

#include "FSettings.h"

FSettings::FSettings() : _localSettings("feroxills.org","Feroxills") {
    _timeDiv = _localSettings.value("timeDiv",0.001).toDouble();
    _ch1VoltDiv = _localSettings.value("ch1VoltDiv",4).toDouble();
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

//Temporaire
