//
// Created by ferdinand on 8/17/26.
//


#include "SerialProvider.h"

#include <iostream>
#include <QDebug>

SerialProvider::SerialProvider(QObject *parent) : ISampleProvider(parent),_currentState(TrameState::SEARCH_MAGIC) {
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(_portName);
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    _samples.reserve(512);
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
                        const double sample_voltage = static_cast<double>(sample) * 5.0 / 1023.0;
                        _samples.push_back(sample_voltage);
                        if (_samples.size() >= 511) {
                            emit samplesAvailable(_samples);
                            _samples.clear();
                            qDebug() << "Signal de envoyé ####";
                        }
                        //qDebug() << "samples -> " << sample_voltage;
                        _currentState = TrameState::SEARCH_MAGIC;
                    } else {
                        _currentState = TrameState::WAIT_MSB;
                    }
                    break;
            }
        }
    });
}

SerialProvider::~SerialProvider() {
    _serialPort->close();
}

void SerialProvider::doStartAcquisition(const ProviderSettings* settings) {
    // interval not applicable for hardware acquisition
    _serialPort->open(QSerialPort::ReadOnly);
}

void SerialProvider::doStopAcquisition() {
    _serialPort->close();
}
