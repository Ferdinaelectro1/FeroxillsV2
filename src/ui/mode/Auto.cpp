//
// Created by ferdinand on 11/02/2026.
//

#include "Auto.h"
#include "../../core/analyser/SamplesAnalyser.h"
#include "src/core/FSettings.h"
#include "../../core/FConstantes.h"

/**
 * Retrieve the most recent 2000-sample window for trigger search.
 * Recover the index of the first trigger.
 * Retrieve 512 samples starting from the trigger index in the ring buffer.
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
    // store 2000 + 512 samples from the ring buffer to use later for display
    // do not use the ring buffer directly because analysis may still push data
    // while the trigger index becomes invalid
    const QVector<double> snapshot = ringBuf->getRecentWindows(2000 + display_win_size);
    const QVector<double> researchBuffer = snapshot.mid(0,2000);
    const auto [min, max] = SamplesAnalyser::getMinMaxVoltage(researchBuffer);
    const auto trigger = (min + max) / 2;
    auto firstRisingPos =  SamplesAnalyser::getFirstRisingPos(researchBuffer,trigger);
    if (firstRisingPos.has_value()) {
        const auto idx = firstRisingPos.value();
        // Verify there is enough space after the trigger position
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
