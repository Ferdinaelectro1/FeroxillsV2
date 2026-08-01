//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERTYPE_H
#define FEROXILLS_PROVIDERTYPE_H

#include <QMetaType>

enum class ProviderState {
    SUCCESS,
    ERROR,
    TIMEOUT_ERROR,
};
Q_DECLARE_METATYPE(ProviderState)

enum class ProviderType {
    SOFTWARE_SOURCE,
    UART_SOURCE,
    USB_SOURCE,
    NO_SOURCE
};

#endif //FEROXILLS_PROVIDERTYPE_H