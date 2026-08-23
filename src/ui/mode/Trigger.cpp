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

void TriggerMode::processDisplaySamples(FRingBuf<double, Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *displaySamplesBuff,const size_t display_win_size) {
    if (!_triggerLevel.has_value()) {
        for (int i= 0; i < display_win_size; i++) {
            displaySamplesBuff[i] = 0;
        }
        return;
    }
    if (ringBuf->size() < 2000 + display_win_size) return;
    // store 2000 + 512 samples from the ring buffer for later display
    // do not use the ring buffer directly because analysis may still push data
    // while the trigger index becomes invalid
    if (_trigger_type == TriggerType::SINGLE_SHOOT_TRIGGER) {
        if (_trigger_is_detected) {
            displaySamplesBuff = _oldDisplaySamples.data(); // send the previously captured buffer
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
        /* Fill the first half of display_win_size with zeros so the trigger can be positioned from there.
        *  This keeps the trigger centered on the screen.
        */
        for (int i = 0; i < bufferOffset; i++) {
            displaySamplesBuff[i] = 0;
        }
        const auto idx = firstRisingPos.value();
        // Verify there is enough space after the trigger position
        if (idx + static_cast<int>(display_win_size) <= snapshot.size()) {
            for (unsigned long i = bufferOffset - 1; i < display_win_size; ++i) {
                /* This (i - (bufferOffset - 1)) mapping allows access to the snapshot data as idx + 0, ..., n without invalid indexing. */
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

