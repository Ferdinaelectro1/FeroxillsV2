//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_TRIGGER_H
#define FEROXILLS_TRIGGER_H


#include "../DisplayMode.h"
#include <QDebug>

enum class TriggerType {
    CONTINUOUS_TRIGGER = 0,
    SINGLE_SHOOT_TRIGGER = 1
};

class TriggerMode final : public  FDisplayMode {
    Q_OBJECT
public:
    TriggerMode();
    void processDisplaySamples(FRingBuf<double,10000> *ringBuf,double *displaySamples, size_t display_win_size) override;
    [[nodiscard]] DisplayMode  getModeType() const  override {
        return TRIGGER;
    }
    Q_INVOKABLE void setTriggerLevel(double triggerLevel);
    Q_INVOKABLE [[nodiscard]] double getTriggerLevel() const;

private:
    std::optional<double> _triggerLevel;
    bool _trigger_is_detected = false;
    TriggerType _trigger_type;
    QVector<double> _oldDisplaySamples = {0};
};


#endif //FEROXILLS_TRIGGER_H