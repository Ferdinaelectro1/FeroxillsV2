//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_CONTINU_H
#define FEROXILLS_CONTINU_H

#include <qlogging.h>
#include <QDebug>

#include  "../DisplayMode.h"

class ContinuMode final : public  FDisplayMode {
    public:
      void processDisplaySamples(double *displaySamples, size_t size) override {
          qDebug() << "ContinuMode";
      }
      [[nodiscard]] DisplayMode  getModeType() const  override {
          return CONTINU;
      }
};


#endif //FEROXILLS_CONTINU_H