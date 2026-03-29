//
// Created by ferdinand on 29/03/2026.
//

#include "SerialWorker.h"

#include <iostream>
#include <QDebug>

SerialWorker::SerialWorker(QObject *parent) : QObject(parent),_currentState(TrameState::SEARCH_MAGIC) {
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(_portName);
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    connect(_serialPort,&QSerialPort::readyRead,this,[&]() {
       const QByteArray data = _serialPort->readAll();
        for (const char c : data) {
            const auto byte = static_cast<uint8_t>(c);
            switch (_currentState) {
                case TrameState::SEARCH_MAGIC:
                    if (byte == 0xFF) {
                        _currentState = TrameState::WAIT_MSB;
                    }
                    break;
                case TrameState::WAIT_MSB:
                    if (byte != 0xFF) {
                        _pendingMsb = byte;
                        _currentState = TrameState::WAIT_LSB;
                    } else {
                        _currentState = TrameState::WAIT_MSB;
                    }
                    break;
                case TrameState::WAIT_LSB:
                    if (byte != 0xFF) {
                        const uint16_t sample = (static_cast<uint16_t>(_pendingMsb) << 3) | static_cast<uint16_t>(byte);
                        qDebug() << "samples -> " << sample;
                        _currentState = TrameState::SEARCH_MAGIC;
                    } else {
                        _currentState = TrameState::WAIT_MSB;
                    }
                    break;
            }
        }
    });
    _serialPort->open(QSerialPort::ReadOnly);
}

SerialWorker::~SerialWorker() {
    _serialPort->close();
}
