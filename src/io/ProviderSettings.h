//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERSETTINGS_H
#define FEROXILLS_PROVIDERSETTINGS_H

class ProviderSettings {
    public:
    virtual  ~ProviderSettings() = default;
    [[nodiscard]] virtual  ProviderSettings* clone() const = 0;
};

#endif //FEROXILLS_PROVIDERSETTINGS_H