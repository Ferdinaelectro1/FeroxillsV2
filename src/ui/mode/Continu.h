//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_CONTINU_H
#define FEROXILLS_CONTINU_H

#include  "../DisplayMode.h"

class ContinuMode final : public  FDisplayMode {
    public:
      void processDisplaySamples(FRingBuf<double,Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *displaySamples, size_t display_win_size) override;
      [[nodiscard]] DisplayMode  getModeType() const  override {
          return CONTINU;
      }
};


#endif //FEROXILLS_CONTINU_H