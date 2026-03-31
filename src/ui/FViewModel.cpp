//
// Created by ferdinand on 31/03/2026.
//

#include "FViewModel.h"
#include "src/core/FConstantes.h"

FViewModel::FViewModel(const double verticalScale,
                       QObject *parent) : QObject(parent),
                                          _verticalScale(verticalScale)
{

}

QVector<QPointF> FViewModel::displayValues() const {
    return _displayValues;
}

void FViewModel::setSamples(const QVector<double> &samples) {
    _samples = samples;
    _displayValues.clear();
    /*
     * Normalise values computing
     */
    for (int i = 0; i < _samples.size(); i++) {
        const auto xNormalise = static_cast<double>(i) / (_samples.size() - 1);
        const double Yrange = _verticalScale * Feroxills::Constants::VERTICAL_DIVISIONS / 2;
        const auto yNormalise = (_samples[i]) / Yrange;
        _displayValues.append(QPointF(xNormalise, yNormalise));
    }
    emit displayValuesChanged();
}

void FViewModel::setVerticalScales(const double verticalScale) {
    _verticalScale = verticalScale;
}





