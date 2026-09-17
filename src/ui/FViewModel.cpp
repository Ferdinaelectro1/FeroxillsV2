//
// Created by ferdinand on 31/03/2026.
//

#include "FViewModel.h"
#include "src/core/FConstantes.h"
#include "src/core/FSettings.h"

FViewModel::FViewModel(const double verticalScale,
                       QObject *parent) : QObject(parent),
                                          _verticalScale(verticalScale)
{

}

QVector<QPointF> FViewModel::displayValues() const {
    return _displayValues;
}

void FViewModel::setWindow(const QVector<double> &window) {
    _windows_display.clear();
    _size_of_window_display = Feroxills::Constants::HORIZONTAL_DIVISIONS * (FSettings::instance()->getTimeDiv() / Feroxills::Constants::SAMPLING_PERIOD);
    if (_size_of_window_display > window.size()) {
        qWarning() << "window that want to be display is more than real available window";
        return;
    }
    for (int i = _display_samples_start_index; i < _display_samples_start_index+_size_of_window_display; i++) {
        _windows_display.append(window[i]);
    }
    _displayValues.clear();
    /*
     * Normalise values computing
     */
    for (int i = 0; i < _windows_display.size(); i++) {
        const auto xNormalise = static_cast<double>(i) / (_windows_display.size() - 1);
        const double Yrange = _verticalScale * Feroxills::Constants::VERTICAL_DIVISIONS / 2;
        const auto yNormalise = (_windows_display[i]) / Yrange;
        _displayValues.append(QPointF(xNormalise, yNormalise));
    }
    emit displayValuesChanged();
}

void FViewModel::setVerticalScales(const double verticalScale) {
    _verticalScale = verticalScale;
}





