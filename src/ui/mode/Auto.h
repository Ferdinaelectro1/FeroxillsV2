//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_AUTO_H
#define FEROXILLS_AUTO_H

#include "../DisplayMode.h"
#include <QDebug>

class AutoMode final : public  FDisplayMode {
public:
    void processDisplaySamples(double *displaySamples, size_t size) override {
        qDebug() << "AutoMode";
    }
};


#endif //FEROXILLS_AUTO_H