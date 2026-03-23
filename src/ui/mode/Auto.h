//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_AUTO_H
#define FEROXILLS_AUTO_H

#include "../DisplayMode.h"
#include <QDebug>

class AutoMode final : public  FDisplayMode {
public:
    void processDisplaySamples(FRingBuf<double,10000> *ringBuf, double *displaySamples, size_t display_win_size) override;
    [[nodiscard]] DisplayMode  getModeType() const  override {
        return AUTO;
    }
private:
    void applyAutoScale(const QVector<double>& snapshot);
    bool _scaled = false;
    float _currentHorizontalScale = 0.0;
};


#endif //FEROXILLS_AUTO_H