//
// Created by ferdinand on 11/02/2026.
//

#include "Trigger.h"
#include "../../core/analyser/SamplesAnalyser.h"


void TriggerMode::processDisplaySamples(FRingBuf<double, 10000> *ringBuf, double *displaySamplesBuff,const size_t display_win_size) {
    if (ringBuf->size() < 2000 + display_win_size) return;
    //on mémorise 2000 + 512 echantillons issues du ring qu'on va use
    //plus tard pour l'affichage, ne pas use le ring buffer directement
    //car le temps que l'analyse finissent des push pourrait survenir
    // et alors l'index trouvé devient invalide
    const QVector<double> snapshot = ringBuf->getRecentWindows(2000 + display_win_size);
    const QVector<double> researchBuffer = snapshot.mid(0,2000);
    auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer,4.5);
    if (firstRisingPos.has_value()) {
        const auto idx = firstRisingPos.value();
        //Verification qu'il y a assez d'espace après la position du trigger
        if (idx + static_cast<int>(display_win_size) <= snapshot.size()) {
            for (unsigned long i = 0; i < display_win_size; i++) {
                displaySamplesBuff[i] = snapshot[idx + i];
            }
        }
    }else {
        qWarning() << "[ERROR] :  Aucun index de front montant trouvé";
    }
}