//
// Created by ferdinand on 03/03/2026.
//

#include "debug.h"
#include <iostream>
#include <QDateTime>
#include <qstring.h>

void debugMessageHandler(const QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    const auto dateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    std::cout << "[" << dateTime.toStdString() << "]";
    switch (type) {
        case QtDebugMsg:
            std::cout <<"["<<ANSI_GREEN<<"INFO"<<ANSI_RESET<<"] ";
            break;
        case QtWarningMsg:
            std::cout << "["<<ANSI_ORANGE<<"WARNING"<<ANSI_RESET<<"] ";
            break;
        case QtFatalMsg:
        case QtCriticalMsg:
            std::cout << "["<<ANSI_RED<<"ERROR"<<ANSI_RESET<<"] ";
            break;
        default:
            break;
    }
    std::cout << msg.toStdString() << std::endl;
}
