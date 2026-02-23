//
// Created by ferdinand on 14/02/2026.
//

#ifndef FEROXILLS_SIGNALANALYSER_H
#define FEROXILLS_SIGNALANALYSER_H

#include <QVector>

struct SamplesParameter {
    double voltage_max;
    double voltage_min;
    unsigned long periodSamples;
    unsigned long _first_rising_pos;
    unsigned long _second_rising_pos;
    bool isPeriodic;
};

class SamplesAnalyser {
public:
    SamplesAnalyser();
    [[nodiscard]] SamplesParameter getSamplesParameter(const QVector<double>& samplesWindows);
    [[nodiscard]] static std::optional<unsigned long> getFirstRisingPos(const QVector<double>& buffer,double hysteresis = 0.0) ;

private:
    //Methodes
    [[nodiscard]] std::pair<double,double> getMinMaxVoltage() const;
    [[nodiscard]] unsigned long getPeriodSamples() const;
    [[nodiscard]] std::pair<unsigned long,unsigned long> getTwoRisingPos() const;
    //Attributs
    SamplesParameter _samplesParameter;
    QVector<double> _samplesWindows;
    double _hysteresis;
};


#endif //FEROXILLS_SIGNALANALYSER_H