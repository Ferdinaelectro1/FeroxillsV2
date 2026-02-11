//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_CONTINU_H
#define FEROXILLS_CONTINU_H

#include <qlogging.h>
#include <QDebug>

#include  "../DisplayMode.h"

class ContinuMode  : public  FDisplayMode {
    public:
      void processDisplaySamples(double *displaySamples, size_t size) override {
          qDebug() << "ContinuMode";
      }
};


#endif //FEROXILLS_CONTINU_H