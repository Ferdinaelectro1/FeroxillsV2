//
// Created by ferdinand on 31/03/2026.
//

#ifndef FEROXILLS_FVIEWMODEL_H
#define FEROXILLS_FVIEWMODEL_H
#include <QObject>
#include <QVector>
#include <QPointF>

class FViewModel final : public  QObject {
      Q_OBJECT
      Q_PROPERTY(QVector<QPointF> displayValues READ displayValues NOTIFY displayValuesChanged);

public:
      explicit FViewModel(double verticalScale,
                          QObject *parent = nullptr);
      [[nodiscard]] QVector<QPointF> displayValues() const;
      void setSamples(const QVector<double>& samples);
      void setVerticalScales(double verticalScale);

      signals:
      void displayValuesChanged();

private:
      double _verticalScale;
      QVector<QPointF> _displayValues;
      QVector<double> _samples;
};

#endif //FEROXILLS_FVIEWMODEL_H