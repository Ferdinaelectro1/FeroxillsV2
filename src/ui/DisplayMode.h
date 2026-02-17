//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_DISPLAYMODE_H
#define FEROXILLS_DISPLAYMODE_H

#include <qobject.h>
#include "../core/FBuffer.h"

class FDisplayMode : public QObject {
    Q_OBJECT
    public:
      ~FDisplayMode() override = default;
      virtual void processDisplaySamples(FRingBuf<double,10000> *ringBuf, double *displaySamples, size_t display_win_size) = 0;
    enum DisplayMode {
        AUTO,
        CONTINU,
        TRIGGER,
        UNKNOW
    };
    [[nodiscard]] virtual DisplayMode getModeType() const = 0;
    Q_ENUM(DisplayMode)
};

#endif //FEROXILLS_DISPLAYMODE_H