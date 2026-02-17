//
// Created by ferdinand on 14/02/2026.
//

#include "SamplesAnalyser.h"

SamplesAnalyser::SamplesAnalyser() : _samplesParameter {0,0,0,0,0},_hysteresis(0.0){}

[[nodiscard]] SamplesParameter SamplesAnalyser::getSamplesParameter(const QVector<double>& samplesWindows) {
    _samplesWindows = samplesWindows;
    const auto [voltage_min, voltage_max] = getMinMaxVoltage();
    _samplesParameter.voltage_min = voltage_min;
    _samplesParameter.voltage_max = voltage_max;
    const auto [firstRisingPos, secondRisingPos] = getTwoRisingPos();
    _samplesParameter._first_rising_pos = firstRisingPos;
    _samplesParameter._second_rising_pos = secondRisingPos;
    _samplesParameter.periodSamples = getPeriodSamples(); //déterminer les front avant de detecter la période
    if (!secondRisingPos || (firstRisingPos == secondRisingPos)) _samplesParameter.isPeriodic = false;
    else _samplesParameter.isPeriodic = true;
    return _samplesParameter;
}

std::pair<double, double> SamplesAnalyser::getMinMaxVoltage() const {
    if (_samplesWindows.empty()) return  {0.0,0.0};
    double MaxVoltage = _samplesWindows[0];
    double MinVoltage = _samplesWindows[0];
    for (const double _sample : _samplesWindows) {
        if (_sample > MaxVoltage) MaxVoltage = _sample;
        if (_sample < MinVoltage) MinVoltage = _sample;
    }
    return {MinVoltage, MaxVoltage};
}

unsigned long SamplesAnalyser::getPeriodSamples() const {
    if (_samplesParameter._first_rising_pos == _samplesParameter._second_rising_pos) {
        return 0.0;
    }
    const double period = _samplesParameter._second_rising_pos - _samplesParameter._first_rising_pos;
    return period;
}

std::pair<unsigned long, unsigned long> SamplesAnalyser::getTwoRisingPos() const {
    if (_samplesWindows.empty()) return {0,0};
    unsigned long firstRisingPos = 0;
    unsigned long secondRisingPos = 0;
    for (int i = 0; i < _samplesWindows.size() - 1; ++i) {
        if (_samplesWindows[i] < -_hysteresis && _samplesWindows[i+1] > _hysteresis) {
            firstRisingPos = i;
            break;
        }
    }
    for (int i = firstRisingPos + 1 ; i < _samplesWindows.size() - 1; ++i) {
        if (_samplesWindows[i] < -_hysteresis && _samplesWindows[i+1] > _hysteresis) {
            secondRisingPos = i;
            break;
        }
    }
    return {firstRisingPos,secondRisingPos};
}

//Récupère la position du premier front montant dans un buffer qu'on lui fournit
std::optional<unsigned long> SamplesAnalyser::getFirstRisingPos(const QVector<double>& buffer) {
    constexpr double hysteresis = 0.0;
    for (int i = 0; i < buffer.size() - 1; ++i) {
        if (buffer[i] < -hysteresis && buffer[i+1] > hysteresis) {
            return  i;
        }
    }
    return std::nullopt;
}