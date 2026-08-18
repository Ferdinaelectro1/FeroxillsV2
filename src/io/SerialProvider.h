//
// Created by ferdinand on 8/17/26.
//

#ifndef FEROXILLS_SERIALPROVIDER_H
#define FEROXILLS_SERIALPROVIDER_H

#include <QSerialPort>
#include <memory>

#include "ISampleProvider.h"

class SerialSettings final : public ProviderSettings {
    Q_OBJECT
    Q_PROPERTY(QString portName READ getPortName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QSerialPort::BaudRate baudRate READ getBaudRate WRITE setBaudRate NOTIFY baudRateChanged)

    public:
    [[nodiscard]] ProviderSettings* clone() const override {
        auto * new_settings = new SerialSettings;
        new_settings->setBaudRate(this->_baudRate);
        new_settings->setPortName(this->_portName);
        new_settings->setMaxVoltage(this->_maxVoltage);
        new_settings->setResolution(this->_resolution);
        return new_settings;
    }

    void setPortName(const QString& name) {_portName = name; emit portNameChanged(); emit anyFieldChanged();}
    void setBaudRate(const QSerialPort::BaudRate speed) {
        _baudRate = speed;
        emit baudRateChanged();
        emit anyFieldChanged();
    }
    void setMaxVoltage(const double voltage) {_maxVoltage = voltage; emit  maxVoltageChanged(); emit anyFieldChanged();}
    void setResolution(const double resolution) {_resolution = resolution; emit resolutionChanged(); emit anyFieldChanged();}

    [[nodiscard]] QString getPortName() const {return _portName;}
    [[nodiscard]] QSerialPort::BaudRate getBaudRate() const {return _baudRate;}
    [[nodiscard]] double getMaxVoltage() const {return _maxVoltage;}
    [[nodiscard]] double getResolution() const {return _resolution;}

    signals:
    void portNameChanged();
    void baudRateChanged();
    void maxVoltageChanged();
    void resolutionChanged();

private:
    QString _portName = "/dev/ttyACM0";
    QSerialPort::BaudRate _baudRate = QSerialPort::Baud9600;
    double _maxVoltage = 5;
    double _resolution = 1024;
};

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
    QVector<double> _samples;
    std::unique_ptr<const SerialSettings> _settings;

    //Method override de interface
    void doStartAcquisition(const ProviderSettings* settings) override;
    void doStopAcquisition() override;
    void doModifyAcquisitionSettings(const ProviderSettings* settings) override;

    void connect_acquisitionSignal();
};


#endif //FEROXILLS_SERIALPROVIDER_H