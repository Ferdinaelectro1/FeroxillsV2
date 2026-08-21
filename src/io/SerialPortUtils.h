//
// Created by ferdinand on 8/21/26.
//

#ifndef FEROXILLS_SERIALPORTUTILS_H
#define FEROXILLS_SERIALPORTUTILS_H
#include  <QSerialPort>
#include  <QSerialPortInfo>
#include  <QVariant>

class SerialPortUtils : public QObject {
    Q_OBJECT
public:
    explicit SerialPortUtils(QObject *parent = nullptr) : QObject(parent) {}
    Q_INVOKABLE static QVariantList getAvailablePorts() {
        QVariantList availablePorts;
        const auto infos = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &info : infos) {
            QVariantMap portInfo;
            portInfo["portName"] = info.portName();
            portInfo["description"] = info.description();
            portInfo["manufacturer"] = info.manufacturer();
            portInfo["systemLocation"] = info.systemLocation();
            availablePorts.append(portInfo);
        }
        return availablePorts;
    }
    static QString getDefaultPortName() {
        const auto ports = QSerialPortInfo::availablePorts();
        if (!ports.isEmpty()) {
            return ports.first().portName();
        }
        return "";
    }
};

#endif //FEROXILLS_SERIALPORTUTILS_H