//
// Created by ferdinand on 11/02/2026.
//

#include "Auto.h"
#include "../../core/analyser/SamplesAnalyser.h"

/**
 *récupération de la fenetre de 2000 element les plus récents pour la recherche du trigger.
 * récupération de l'index du premier trigger
 * Récupération de 512 éléments en partant de l'index du premier trigger dans le ring
*/
void AutoMode::processDisplaySamples(FRingBuf<double, 10000> *ringBuf, double *displaySamples,const size_t display_win_size) {
    if (ringBuf->size() > 2000) {
        const QVector<double> researchBuffer = ringBuf->getRecentWindows(2000);
        auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer);
        if (firstRisingPos.has_value()) {
            const unsigned long realFirstRinsingPosIndex  = (10000 - 2000) + firstRisingPos.has_value();
            ringBuf->getWindowFromIndex(realFirstRinsingPosIndex,displaySamples,512);
        }else {
            qWarning() << "[ERROR] :  Aucun index de front montant trouvé";
        }
    }
}
