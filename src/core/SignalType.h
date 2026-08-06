//
// Created by ferdinand on 8/6/26.
//

#ifndef FEROXILLS_SIGNALTYPE_H
#define FEROXILLS_SIGNALTYPE_H
#include <QObject>

class SignalType final : public QObject {
    Q_OBJECT
    public:
        enum Type {
            SINUS,
            CARRE,
            TRIANGLE,
            CONTINU,
            RAMPE,
            RANDOM,
            PWM,
            RISING_PULSE,
            AM,
            FM,
            ZERO
        };
    Q_ENUM(Type)
};

#endif //FEROXILLS_SIGNALTYPE_H