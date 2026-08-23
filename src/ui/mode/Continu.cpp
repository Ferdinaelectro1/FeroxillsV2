//
// Created by ferdinand on 11/02/2026.
//

#include "Continu.h"

void ContinuMode::processDisplaySamples(FRingBuf<double, Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *displaySamples,const size_t display_win_size) {
    ringBuf->advanceRead(5); // advance by 5 elements
    ringBuf->getWindow(displaySamples,display_win_size); // copy the new display_win_size data from the ring buffer to the display buffer
}
