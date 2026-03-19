//
// Created by ferdinand on 13/03/2026.
//

#ifndef FEROXILLS_FSETTINGS_H
#define FEROXILLS_FSETTINGS_H
#include <qobject.h>
#include <QSettings>


class FSettings final : public QObject {
    Q_OBJECT
    Q_PROPERTY(double timeDiv READ getTimeDiv WRITE setTimeDiv NOTIFY onTimeDivChanged)
    Q_PROPERTY(double ch1VoltDiv READ getCh1VoltDiv WRITE setCh1VoltDiv NOTIFY onCh1VoltDivChanged)
public:
    static FSettings *instance();
    FSettings(const FSettings &) = delete;
    FSettings &operator=(const FSettings &) = delete;
    FSettings(const FSettings &&) = delete;
    FSettings &operator=(const FSettings &&) = delete;
    [[nodiscard]] double getTimeDiv() const;
    [[nodiscard]] double getCh1VoltDiv() const;
    [[nodiscard]] unsigned long getSamplesNeeded() const;
    void setTimeDiv(double timeDiv);
    void setCh1VoltDiv(double ch1VoltDiv);
    void setSamplesNeeded(unsigned long samplesNeeded);

    signals:
    void onTimeDivChanged();
    void onCh1VoltDivChanged();

private:
    FSettings();
    double _timeDiv;
    double _ch1VoltDiv;
    QSettings _localSettings;
    unsigned long _sample_needed;
};

#endif //FEROXILLS_FSETTINGS_H