//
// Created by ferdinand on 11/02/2026.
//

#include "Trigger.h"
#include "../../core/analyser/SamplesAnalyser.h"
#include "../../core/event/EventBus.h"
#include "src/core/FSettings.h"


TriggerMode::TriggerMode() {
    emit EventBus::getInstance()->TriggerModeDisplayInvoked(this);
    _trigger_type = static_cast<TriggerType>(FSettings::instance()->getTriggerModeTriggerType());
    qDebug() << "TriggerMode::TriggerMode" << FSettings::instance()->getTriggerModeTriggerType();
    connect(FSettings::instance(),&FSettings::onTriggerMode_triggerTypeChanged,this, [&]() {
        _trigger_type = static_cast<TriggerType>(FSettings::instance()->getTriggerModeTriggerType());
    });
}

void TriggerMode::processDisplaySamples(FRingBuf<double, 10000> *ringBuf, double *displaySamplesBuff,const size_t display_win_size) {
    if (!_triggerLevel.has_value()) {
        for (int i= 0; i < display_win_size; i++) {
            displaySamplesBuff[i] = 0;
        }
        return;
    }
    if (ringBuf->size() < 2000 + display_win_size) return;
    //on mémorise 2000 + 512 echantillons issues du ring qu'on va use
    //plus tard pour l'affichage, ne pas use le ring buffer directement
    //car le temps que l'analyse finissent des push pourrait survenir
    // et alors l'index trouvé devient invalide
    if (_trigger_type == TriggerType::SINGLE_SHOOT_TRIGGER) {
        if (_trigger_is_detected) {
            displaySamplesBuff = _oldDisplaySamples.data(); //on envoie l'ancien buffer capturé
            return;
        }
    }

    const QVector<double> snapshot = ringBuf->getRecentWindows(2000 + display_win_size);
    const QVector<double> researchBuffer = snapshot.mid(0,2000);
    auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer,_triggerLevel.value());
    if (firstRisingPos.has_value()) {
        if (_trigger_type == TriggerType::SINGLE_SHOOT_TRIGGER) {
            _oldDisplaySamples.clear();
            _trigger_is_detected = true;
        }
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
            for (unsigned long i = bufferOffset - 1; i < display_win_size; ++i) {
                /*On fait ceci (i - (bufferOffset - 1)) pour que on puisse accéder dans le snapshot les données ainsin :  id x+ 0 , ..., n . Pour éviter des mauvais accèes*/
                displaySamplesBuff[i] = snapshot[idx + (i - (bufferOffset - 1))];
                if (_trigger_type == TriggerType::SINGLE_SHOOT_TRIGGER) {
                    _oldDisplaySamples.append(displaySamplesBuff[i]);
                }
            }
        }
    }else {
        for (int i= 0; i < display_win_size; i++) {
            displaySamplesBuff[i] = 0;
        }
    }
}

void TriggerMode::setTriggerLevel(const double triggerLevel) {
    qDebug() << "Setting trigger level to " << triggerLevel;
    _triggerLevel = triggerLevel;
}

double TriggerMode::getTriggerLevel() const {
    return  _triggerLevel.value();
}

