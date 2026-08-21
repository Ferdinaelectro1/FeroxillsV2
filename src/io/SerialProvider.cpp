//
// Created by ferdinand on 8/17/26.
//


#include "SerialProvider.h"

#include <iostream>
#include <QDebug>

#include "SerialPortUtils.h"

SerialSettings::SerialSettings() {
    _portName = SerialPortUtils::getDefaultPortName();
}

SerialProvider::SerialProvider(QObject *parent) : ISampleProvider(parent),_currentState(TrameState::SEARCH_MAGIC) {
    _serialPort = new QSerialPort(this);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    connect_acquisitionSignal();
    _samples.reserve(512);
    _settings = nullptr;
}

SerialProvider::~SerialProvider() {
    _serialPort->close();
}

void SerialProvider::doStartAcquisition(const ProviderSettings* settings) {
    auto new_settings =  dynamic_cast<const SerialSettings *>(settings);
    if (!new_settings) {
        qWarning() << Q_FUNC_INFO << "this ProviderSettings in parameter is not SerialSettings";
        return;
    }
    _settings =  std::unique_ptr<const SerialSettings>(dynamic_cast<const SerialSettings *>(settings->clone()));
    _serialPort->setPortName(_settings->getPortName());
    _serialPort->setBaudRate(_settings->getBaudRate());
    _serialPort->open(QSerialPort::ReadOnly);
}

void SerialProvider::doStopAcquisition() {
    _serialPort->close();
}

void SerialProvider::doModifyAcquisitionSettings(const ProviderSettings *settings) {
    auto new_settings =  dynamic_cast<const SerialSettings *>(settings);
    if (!new_settings) {
        qWarning() << Q_FUNC_INFO << "this ProviderSettings in parameter is not SerialSettings";
        return;
    }
    QString  oldPortName = "";
    if (_settings) {
        oldPortName = _settings->getPortName();
    }
    _settings =  std::unique_ptr<const SerialSettings>(dynamic_cast<const SerialSettings *>(settings->clone()));
    if (oldPortName != _settings->getPortName()) {
        _serialPort->close();
        _serialPort->setPortName(_settings->getPortName());
        _serialPort->open(QSerialPort::ReadOnly);
    }
    _serialPort->setBaudRate(_settings->getBaudRate());
}

void SerialProvider::connect_acquisitionSignal() {
    if (!_serialPort) return;
    connect(_serialPort,&QSerialPort::readyRead,this,[this]() {
        if (!_settings) return;
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
                        const double sample_voltage = static_cast<double>(sample) * _settings->getMaxVoltage() / _settings->getResolution();
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
