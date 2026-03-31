//
// Created by ferdinand on 11/02/2026.
//

#include "Auto.h"
#include "../../core/analyser/SamplesAnalyser.h"
#include "src/core/FSettings.h"
#include "../../core/FConstantes.h"

/**
 *récupération de la fenetre de 2000 element les plus récents pour la recherche du trigger.
 * récupération de l'index du premier trigger
 * Récupération de 512 éléments en partant de l'index du premier trigger dans le ring
*/

void AutoMode::applyAutoScale (const QVector<double>& snapshot) {
    if (_scaled) return;
    const auto [min, max] = SamplesAnalyser::getMinMaxVoltage(snapshot);
    const double marge_of_voltage = max - min;
    const auto vPerDiv = static_cast<float>(marge_of_voltage/Feroxills::Constants::VERTICAL_DIVISIONS) ;//
    const auto suggetvPerDiv = SamplesAnalyser::getVerticalAdaptedScale(vPerDiv);
    const auto signalPeriod = SamplesAnalyser::getPeriod(snapshot);
    if (signalPeriod.has_value()) {
        const auto suggestPeriod = (signalPeriod.value() * Feroxills::Constants::NUMBER_OF_PERIOD_PRINT_IN_AUTO) / Feroxills::Constants::HORIZONTAL_DIVISIONS;
        FSettings::instance()->setTimeDiv(SamplesAnalyser::getHorizontalAdaptedScale(static_cast<float>(suggestPeriod),_currentHorizontalScale));
    }
    _scaled = true;
    FSettings::instance()->setCh1VoltDiv(suggetvPerDiv);
}

void AutoMode::processDisplaySamples(FRingBuf<double, Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *displaySamplesBuff,const size_t display_win_size) {
    if (ringBuf->size() < 2000 + display_win_size) return;
    //on mémorise 2000 + 512 echantillons issues du ring qu'on va use
    //plus tard pour l'affichage, ne pas use le ring buffer directement
    //car le temps que l'analyse finissent des push pourrait survenir
    // et alors l'index trouvé devient invalide
    const QVector<double> snapshot = ringBuf->getRecentWindows(2000 + display_win_size);
    const QVector<double> researchBuffer = snapshot.mid(0,2000);
    const auto [min, max] = SamplesAnalyser::getMinMaxVoltage(researchBuffer);
    const auto trigger = (min + max) / 2;
    auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer,trigger);
    if (firstRisingPos.has_value()) {
        const auto idx = firstRisingPos.value();
        //Verification qu'il y a assez d'espace après la position du trigger
        if (idx + static_cast<int>(display_win_size) <= snapshot.size()) {
            for (unsigned long i = 0; i < display_win_size; ++i) {
                displaySamplesBuff[i] = snapshot[idx + i];
            }
        }
    }else {
        for (int i= 0; i < display_win_size; i++) {
            displaySamplesBuff[i] = snapshot[snapshot.size() - (display_win_size - i)];
        }
    }
    applyAutoScale(snapshot);
}
