//
// Created by ferdinand on 8/17/26.
//

#ifndef FEROXILLS_SERIALPROVIDER_H
#define FEROXILLS_SERIALPROVIDER_H

#include <QSerialPort>

#include "ISampleProvider.h"

class SerialProvider final : public ISampleProvider {
    Q_OBJECT

public:
    explicit SerialProvider(QObject *parent = nullptr);
    ~SerialProvider() override;

private:
    enum class TrameState {
        SEARCH_MAGIC,
        WAIT_MSB,
        WAIT_LSB
    };
    TrameState _currentState;
    uint8_t _pendingMsb;
    QSerialPort *_serialPort;
    QString _portName = "/dev/ttyACM0";
    QVector<double> _samples;

    //Method override de interface
    void doStartAcquisition(const ProviderSettings* settings) override;
    void doStopAcquisition() override;
};


#endif //FEROXILLS_SERIALPROVIDER_H