//
// Created by ferdinand on 14/02/2026.
//

#include "SamplesAnalyser.h"
#include "../FConstantes.h"

SamplesAnalyser::SamplesAnalyser() : _samplesParameter {0,0,0,0,0},_hysteresis(0.0){}

[[nodiscard]] SamplesParameter SamplesAnalyser::getSamplesParameter(const QVector<double>& samplesWindows) {
    _samplesWindows = samplesWindows;
    const auto [voltage_min, voltage_max] = getMinMaxVoltage();
    _samplesParameter.voltage_min = voltage_min;
    _samplesParameter.voltage_max = voltage_max;
    constexpr  unsigned long firstRisingPos = 0;
    constexpr  unsigned long secondRisingPos = 30;
    _samplesParameter._first_rising_pos = firstRisingPos;
    _samplesParameter._second_rising_pos = secondRisingPos;
    _samplesParameter.periodSamples = getPeriodSamples(); //déterminer les front avant de detecter la période
    _samplesParameter.isPeriodic = true;
    return _samplesParameter;
}

std::pair<double, double> SamplesAnalyser::getMinMaxVoltage() const {
    return getMinMaxVoltage(_samplesWindows);
}

unsigned long SamplesAnalyser::getPeriodSamples() const {
    if (_samplesParameter._first_rising_pos == _samplesParameter._second_rising_pos) {
        return 0.0;
    }
    const unsigned long period = _samplesParameter._second_rising_pos - _samplesParameter._first_rising_pos;
    return period;
}

static std::pair<unsigned long, unsigned long> getTwoRisingPos(const QVector<double>& samples) {
    if (samples.empty()) return {0,0};
    unsigned long firstRisingPos = 0;
    unsigned long secondRisingPos = 0;
    for (int i = 0; i < samples.size() - 1; ++i) {
        if (samples[i] < 0 && samples[i+1] > 0) {
            firstRisingPos = i;
            break;
        }
    }
    for (int i = firstRisingPos + 1 ; i < samples.size() - 1; ++i) {
        if (samples[i] < 0 && samples[i+1] > 0) {
            secondRisingPos = i;
            break;
        }
    }
    return {firstRisingPos,secondRisingPos};
}

//Récupère la position du premier front montant dans un buffer qu'on lui fournit
std::optional<unsigned long> SamplesAnalyser::getFirstRisingPos(const QVector<double>& buffer,const double trigger) {
    for (int i = 0; i < buffer.size() - 1; ++i) {
        if (buffer[i] <= trigger && buffer[i+1] > trigger) {
            return  i;
        }
    }
    return std::nullopt;
}

std::optional<double> SamplesAnalyser::getPeriod(const QVector<double> &samples) {
    const auto [firstRisingPos, secondRisingPos] = getTwoRisingPos(samples);
    if (firstRisingPos == secondRisingPos) {
        return std::nullopt;
    }
    const double period = static_cast<double>(secondRisingPos - firstRisingPos)*(1.0 / Feroxills::Constants::SAMPLING_FREQUENCY);
    return period;
}

std::pair<double, double> SamplesAnalyser::getMinMaxVoltage(const QVector<double> &samples) {
    if (samples.empty()) return  {0.0,0.0};
    double MaxVoltage = samples[0];
    double MinVoltage = samples[0];
    for (const double _sample : samples) {
        if (_sample > MaxVoltage) MaxVoltage = _sample;
        if (_sample < MinVoltage) MinVoltage = _sample;
    }
    return {MinVoltage, MaxVoltage};
}

float SamplesAnalyser::getVerticalAdaptedScale(const float calculatedScale) {
    for (const float scale : Feroxills::Constants::VERTICAL_SCALES) {
        if (calculatedScale <= scale) return scale;
    }
    return calculatedScale;
}

static float getAdaptedScale (const float calculatedScale,float& currentScale) {
    for (const float scale : Feroxills::Constants::HORIZONTAL_SCALES) {
        if (calculatedScale <= scale) {
            currentScale = scale;
            return scale;
        }
    }
    return calculatedScale;
}

float SamplesAnalyser::getHorizontalAdaptedScale(const float calculatedScale,float& currentScale) {
    if (currentScale < std::numeric_limits<float>::epsilon()) {
        return getAdaptedScale(calculatedScale,currentScale);
    }
    /*On vérifie si le scale calculé n'est pas dans la zone d'hystérésis pour éviter des variations brusques*/
    if (calculatedScale <= currentScale*(1 + Feroxills::Constants::SCALE_HYSTERESIS) && (calculatedScale >= currentScale*(1 - Feroxills::Constants::SCALE_HYSTERESIS))) {
        return currentScale;
    }
    return getAdaptedScale(calculatedScale,currentScale);
}