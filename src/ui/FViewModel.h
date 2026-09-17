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
      void setWindow(const QVector<double>& window);
      void setVerticalScales(double verticalScale);

      signals:
      void displayValuesChanged();

private:
      double _verticalScale;
      QVector<QPointF> _displayValues;
      QVector<double> _windows_display; //window that will display on the screen
      qsizetype _size_of_window_display;
      int _display_samples_start_index = 0;
};

#endif //FEROXILLS_FVIEWMODEL_H