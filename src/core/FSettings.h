//
// Created by ferdinand on 13/03/2026.
//

#ifndef FEROXILLS_FSETTINGS_H
#define FEROXILLS_FSETTINGS_H
#include <qobject.h>
#include "event/EventBus.h"


class FSettings final : public QObject {
    Q_OBJECT
    Q_PROPERTY(double voltPerDivision READ getVoltPerDivision NOTIFY onVoltPerDivisionChanged)
    Q_PROPERTY(double timePerDivision READ getTimePerDivision NOTIFY onTimePerDivisionChanged)

public:
    explicit FSettings(QObject *parent = nullptr) : QObject(parent), _voltPerDivision(4),_timePerDivision(0.001){
        connect(EventBus::getInstance(),&EventBus::Suggest_volt_PerDiv,this,&FSettings::setVoltPerDivision);
    }
    [[nodiscard]] double getVoltPerDivision() const {return _voltPerDivision;}
    [[nodiscard]] double getTimePerDivision() const {return _timePerDivision;}

    signals:
    void onVoltPerDivisionChanged();
    void onTimePerDivisionChanged();

public slots:
    void setVoltPerDivision(const double newVoltPerDivision) { _voltPerDivision = newVoltPerDivision; emit onVoltPerDivisionChanged();}
    void setTimePerDivision(const double newTimePerDivision) { _timePerDivision = newTimePerDivision; emit onTimePerDivisionChanged();}

private:
    double _voltPerDivision;
    double _timePerDivision;
};

#endif //FEROXILLS_FSETTINGS_H