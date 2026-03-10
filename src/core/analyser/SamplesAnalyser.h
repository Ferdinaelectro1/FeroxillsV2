//
// Created by ferdinand on 14/02/2026.
//

#ifndef FEROXILLS_SIGNALANALYSER_H
#define FEROXILLS_SIGNALANALYSER_H

#include <QVector>
#define DIVISION_TOTAL 8.0

static constexpr float verticalScale[] = {
    0.01f,0.02f,0.05f,0.1f,0.2f,0.5f,1.0f,2.0f,5.0f,10.0f
};

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
    [[nodiscard]] static std::optional<unsigned long> getFirstRisingPos(const QVector<double>& buffer,double trigger = 0.0) ;
    [[nodiscard]] static  std::pair<double,double> getMinMaxVoltage(const QVector<double>& samples);
    [[nodiscard]] static float getVerticalAdaptedScale(float calculatedScale);

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