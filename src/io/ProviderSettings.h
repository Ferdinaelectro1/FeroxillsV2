//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERSETTINGS_H
#define FEROXILLS_PROVIDERSETTINGS_H

#include <QObject>

class ProviderSettings : public  QObject {
    Q_OBJECT
    public:
    ~ProviderSettings() override = default;
    [[nodiscard]] virtual  ProviderSettings* clone() const = 0;
    signals:
    void anyFieldChanged();
};

#endif //FEROXILLS_PROVIDERSETTINGS_H