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

void AutoMode::processDisplaySamples(FRingBuf<double, Feroxills::Constants::RING_BUFFER_SIZE> *ringBuf, double *window,const size_t window_size) {
    if (ringBuf->size() < 2000 + window_size) return;
    constexpr qsizetype size_of_researchBuffer_in_recent_window = 2000;

    if (_holdoffActive) {
        // "search" state: looking for a new trigger
        uint64_t pushCountAtSnapshot = 0;
        const QVector<double> snapshot = ringBuf->getRecentWindowsWithCount(2000 + window_size, pushCountAtSnapshot);
        const auto [min, max] = SamplesAnalyser::getMinMaxVoltage(snapshot);
        const auto trigger = (min + max) / 2;
        const QVector<double> researchBuffer = snapshot.mid((snapshot.size() -1) - size_of_researchBuffer_in_recent_window, size_of_researchBuffer_in_recent_window);
        const auto firstRisingPos = SamplesAnalyser::getFirstRisingPos(researchBuffer, trigger);

        if (firstRisingPos.has_value()) {
            const auto idx = firstRisingPos.value();
            // account for the samples already elapsed since the trigger, within the small research window
            _pushCountAtTrigger = pushCountAtSnapshot - (size_of_researchBuffer_in_recent_window - idx);
            _holdoffActive = false;
        } else {
            // no trigger found: fall back to roll mode (most recent samples, un-synchronized)
            const auto offset = static_cast<qsizetype>(snapshot.size() - window_size);
            for (size_t i = 0; i < window_size; i++) {
                window[i] = snapshot[offset + static_cast<qsizetype>(i)];
            }
            applyAutoScale(snapshot);
        }
    } else {
        // "filling" state: ignore any new trigger candidate, just count elapsed samples since the accepted trigger
        uint64_t currentTotalPushed = 0;
        currentTotalPushed = ringBuf->totalPushed();
        const size_t currentCount = currentTotalPushed - _pushCountAtTrigger;
        if (currentCount < window_size) {
            return; // valid_count untouched: caller keeps previous stable frame as-is
        }

        uint64_t pushCountAtRead = 0;
        QVector<double> fullWindow;
        if (!ringBuf->tryGetWindowSinceTrigger(_pushCountAtTrigger, window_size, fullWindow)) {
            return; // Not enough data yet; keeping the previous display.
        }
        _holdoffActive = true;
        for (size_t i = 0; i < window_size; i++) {
            window[i] = fullWindow[static_cast<qsizetype>(i)];
        }
        applyAutoScale(fullWindow);
    }
}
