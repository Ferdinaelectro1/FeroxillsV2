//
// Created by ferdinand on 11/02/2026.
//

#include "Continu.h"

void ContinuMode::processDisplaySamples(FRingBuf<double, Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *displaySamples,const size_t display_win_size) {
    ringBuf->advanceRead(5);//on avance de 5 element
    ringBuf->getWindow(displaySamples,display_win_size); //on met les display_win_size nouveaux données issues du ringBuffer dans le buffer d'affichage
}
