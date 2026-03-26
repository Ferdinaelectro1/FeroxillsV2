//
// Created by ferdinand on 13/03/2026.
//

#include "FSettings.h"
#include "FConstantes.h"

static ssize_t getScalePos(const double scale, const float *scaleBuffer, const size_t scaleBufferSize) {
    for (int i = 0; i < scaleBufferSize; i++) {
        if (std::abs(scale - scaleBuffer[i]) < 1e-6f) { //this is equivalent to scale == scaleBuffer[i]
            return i;
        }
    }
    return -1;
}

FSettings::FSettings() : _localSettings("feroxills.org","Feroxills") {
    _timeDiv = _localSettings.value("timeDiv",0.001).toDouble();
    _ch1VoltDiv = _localSettings.value("ch1VoltDiv",4).toDouble();
    _sample_needed = static_cast<unsigned long>( _localSettings.value("samplesNeeded",441).toULongLong());
    _triggerMode_triggerType = _localSettings.value("triggerMode_triggerType",0).toInt();
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

int FSettings::getTriggerModeTriggerType() const {
    return  _triggerMode_triggerType;
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

void FSettings::setTriggerModeTriggerType(const int triggerMode) {
    if (_triggerMode_triggerType != triggerMode) {
        _triggerMode_triggerType = triggerMode;
        qDebug() << "Fsettings::setTriggerModeTriggerType" << triggerMode;
        _localSettings.setValue("triggerMode_triggerType",triggerMode);
        emit onTriggerMode_triggerTypeChanged();
    }
}

void FSettings::stepCh1VoltDiv(const bool decrement) {
    constexpr auto totalScaleSize = std::size(Feroxills::Constants::VERTICAL_SCALES);
    const ssize_t posFound = getScalePos(getCh1VoltDiv(),Feroxills::Constants::VERTICAL_SCALES,totalScaleSize);
    if (posFound != -1) {
        _current_vertical_scale_pos = posFound;
    }
    if (decrement) {
        if (_current_vertical_scale_pos > 0)
           _current_vertical_scale_pos--;
    }
    else {
        if (_current_vertical_scale_pos < totalScaleSize - 1)
          _current_vertical_scale_pos++;
    }
    setCh1VoltDiv(Feroxills::Constants::VERTICAL_SCALES[_current_vertical_scale_pos]);
    //qDebug() << "Pos : "<<_current_vertical_scale_pos<<" Value = "<<Feroxills::Constants::VERTICAL_SCALES[_current_vertical_scale_pos];
}

void FSettings::stepTimeDiv(const bool decrement) {
    constexpr auto totalScaleSize = std::size(Feroxills::Constants::HORIZONTAL_SCALES);
    const ssize_t posFound = getScalePos(getTimeDiv(),Feroxills::Constants::HORIZONTAL_SCALES,totalScaleSize);
    if (posFound != -1) {
        _current_horizontal_scale_pos = posFound;
    }
    if (decrement) {
        if (_current_horizontal_scale_pos > 0)
            _current_horizontal_scale_pos--;
    }
    else {
        if (_current_horizontal_scale_pos < totalScaleSize - 1)
            _current_horizontal_scale_pos++;
    }
    setTimeDiv(Feroxills::Constants::HORIZONTAL_SCALES[_current_horizontal_scale_pos]);
}

void FSettings::decrementCh1VoltDiv() {
   stepCh1VoltDiv(true);
}

void FSettings::incrementCh1VoltDiv() {
    stepCh1VoltDiv(false);
}

void FSettings::incrementTimeDiv() {
    stepTimeDiv(false);
}

void FSettings::decrementTimeDiv() {
    stepTimeDiv(true);
}
//Temporaire
