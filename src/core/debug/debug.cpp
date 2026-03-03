//
// Created by ferdinand on 03/03/2026.
//

#include "debug.h"

debug& debug::instance() {
    static debug instance;
    return instance;
}

void debug::log(const QString &message, const QString &logType) const {
    qDebug() <<"["<<logType<<"]"<< message;
}
