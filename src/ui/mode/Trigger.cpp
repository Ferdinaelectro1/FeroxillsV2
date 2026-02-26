//
// Created by ferdinand on 11/02/2026.
//

#include "Trigger.h"
#include "../../core/analyser/SamplesAnalyser.h"
#include "../../core/event/EventBus.h"


TriggerMode::TriggerMode() {
    emit EventBus::getInstance()->TriggerModeDisplayInvoked(this);
}

void TriggerMode::processDisplaySamples(FRingBuf<double, 10000> *ringBuf, double *displaySamplesBuff,const size_t display_win_size) {
    if (ringBuf->size() < 2000 + display_win_size) return;
    //on mémorise 2000 + 512 echantillons issues du ring qu'on va use
    //plus tard pour l'affichage, ne pas use le ring buffer directement
    //car le temps que l'analyse finissent des push pourrait survenir
    // et alors l'index trouvé devient invalide
    const QVector<double> snapshot = ringBuf->getRecentWindows(2000 + display_win_size);
    const QVector<double> researchBuffer = snapshot.mid(0,2000);
    auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer,_triggerLevel);
    if (firstRisingPos.has_value()) {
         const unsigned int bufferOffset = display_win_size/2;
        /* On remplit les 1 /2 premiers valeurs de display_win_size par des 0, pour pouvoir placer par la suite le trigger en partant de là.
        *  Pour que le trigger soit affiché au centre de l'écran
        */
        for (int i = 0; i < bufferOffset; i++) {
            displaySamplesBuff[i] = 0;
        }
        const auto idx = firstRisingPos.value();
        //Verification qu'il y a assez d'espace après la position du trigger
        if (idx + static_cast<int>(display_win_size) <= snapshot.size()) {
            for (unsigned long i = bufferOffset - 1; i < display_win_size; i++) {
                /*On fait ceci (i - (bufferOffset - 1)) pour que on puisse accéder dans le snapshot les données ainsin :  id x+ 0 , ..., n . Pour éviter des mauvais accèes*/
                displaySamplesBuff[i] = snapshot[idx + (i - (bufferOffset - 1))];
            }
        }
    }else {
        qWarning() << "[ERROR] :  Aucun trigger trouvé";
    }
}

void TriggerMode::setTriggerLevel(const double triggerLevel) {
    qDebug() << "Setting trigger level to " << triggerLevel;
    _triggerLevel = triggerLevel;
}

double TriggerMode::getTriggerLevel() const {
    return  _triggerLevel;
}

