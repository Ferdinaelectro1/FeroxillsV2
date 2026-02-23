//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_TRIGGER_H
#define FEROXILLS_TRIGGER_H


#include "../DisplayMode.h"
#include <QDebug>

class TriggerMode final : public  FDisplayMode {
public:
    void processDisplaySamples(FRingBuf<double,10000> *ringBuf,double *displaySamples, size_t display_win_size) override;
    [[nodiscard]] DisplayMode  getModeType() const  override {
        return TRIGGER;
    }
};


#endif //FEROXILLS_TRIGGER_H