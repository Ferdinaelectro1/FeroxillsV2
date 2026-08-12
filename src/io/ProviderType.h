//
// Created by ferdinand on 8/1/26.
//

#ifndef FEROXILLS_PROVIDERTYPE_H
#define FEROXILLS_PROVIDERTYPE_H

#include <QMetaType>
#include <QObject>

enum class ProviderState {
    SUCCESS,
    ERROR,
    TIMEOUT_ERROR,
};
Q_DECLARE_METATYPE(ProviderState)

class ProviderType final : public QObject {
    Q_OBJECT
    public:
        enum Type {
            SOFTWARE_SOURCE,
            UART_SOURCE,
            USB_SOURCE,
            NO_SOURCE
        };
        Q_ENUM(Type)
};


#endif //FEROXILLS_PROVIDERTYPE_H