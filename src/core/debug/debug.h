//
// Created by ferdinand on 03/03/2026.
//

#ifndef FEROXILLS_DEBUG_H
#define FEROXILLS_DEBUG_H
#include <QVector>
#include <QFile>
#include <QDebug>

#define INFO(msg) debug::instance().log(msg,"DEBUG")
#define ERROR(msg) debug::instance().log(msg,"ERROR")
#define WARN(msg) debug::instance().log(msg,"WARNING")

class debug {
public:
    debug(const debug&) = delete;
    debug(debug&&) = delete;
    debug& operator=(const debug&) = delete;
    debug& operator=(debug&&) = delete;
    static debug& instance();
    void log(const QString& message,const QString& logType) const;
    template<typename Type>
    static void printCurve(const QVector<Type>& fX, const QVector<Type>& fY, const QString& file_name)
    {
        QFile file(file_name) ;
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            const int n = std::min(fX.size(), fY.size());
            for(int i=0; i<n; ++i)
            {
                out << fX[i] << " " << fY[i] << "\n";
            }
        }
        else {
            qDebug() << "[Error] : Cannot open file : "<< file_name ;
        }
    }

private:
    debug() = default;
};

#endif //FEROXILLS_DEBUG_H