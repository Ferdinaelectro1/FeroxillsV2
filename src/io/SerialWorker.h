//
// Created by ferdinand on 29/03/2026.
//

#ifndef FEROXILLS_SERIALWORKER_H
#define FEROXILLS_SERIALWORKER_H

#include <QSerialPort>

class SerialWorker final : public QObject {
    Q_OBJECT

public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker() override;

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
};


#endif //FEROXILLS_SERIALWORKER_H