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
    Q_PROPERTY(int triggerModeTriggerType READ getTriggerModeTriggerType WRITE setTriggerModeTriggerType NOTIFY onTriggerMode_triggerTypeChanged)
public:
    static FSettings *instance();
    FSettings(const FSettings &) = delete;
    FSettings &operator=(const FSettings &) = delete;
    FSettings(const FSettings &&) = delete;
    FSettings &operator=(const FSettings &&) = delete;
    [[nodiscard]] double getTimeDiv() const;
    [[nodiscard]] double getCh1VoltDiv() const;
    [[nodiscard]] unsigned long getSamplesNeeded() const;
    [[nodiscard]] int getTriggerModeTriggerType() const;
    void setTimeDiv(double timeDiv);
    void setCh1VoltDiv(double ch1VoltDiv);
    void setSamplesNeeded(unsigned long samplesNeeded);
    void setTriggerModeTriggerType(int triggerMode); // 0 = continuous, 1 = single shot
    Q_INVOKABLE void incrementCh1VoltDiv();
    Q_INVOKABLE void decrementCh1VoltDiv();
    Q_INVOKABLE void incrementTimeDiv();
    Q_INVOKABLE void decrementTimeDiv();

    signals:
    void onTimeDivChanged();
    void onCh1VoltDivChanged();
    void onTriggerMode_triggerTypeChanged();

private:
    FSettings();
    double _timeDiv;
    double _ch1VoltDiv;
    QSettings _localSettings;
    unsigned long _sample_needed;
    int _triggerMode_triggerType;
    unsigned long _current_vertical_scale_pos = 0;
    unsigned long _current_horizontal_scale_pos = 0;
    void stepCh1VoltDiv(bool decrement);
    void stepTimeDiv(bool decrement);
};

#endif //FEROXILLS_FSETTINGS_H