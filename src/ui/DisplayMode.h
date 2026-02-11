//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_DISPLAYMODE_H
#define FEROXILLS_DISPLAYMODE_H

#include  <cstddef>

enum class DisplayMode {
    AUTO,
    CONTINU,
    TRIGGER,
    UNKNOW
};

class FDisplayMode {
    public:
      virtual  ~FDisplayMode() = default;
      virtual void processDisplaySamples(double *displaySamples, size_t size) = 0;

};

#endif //FEROXILLS_DISPLAYMODE_H