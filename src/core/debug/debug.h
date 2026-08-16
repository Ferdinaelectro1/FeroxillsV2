//
// Created by ferdinand on 03/03/2026.
//

#ifndef FEROXILLS_DEBUG_H
#define FEROXILLS_DEBUG_H
#include <QMessageLogContext>

#define ANSI_RESET   "\033[0m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_ORANGE  "\033[33m"
#define ANSI_RED     "\033[31m"

void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif //FEROXILLS_DEBUG_H