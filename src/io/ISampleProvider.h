//
// Created by ferdinand on 05/03/2026.
//

#ifndef FEROXILLS_ISAMPLESREADER_H
#define FEROXILLS_ISAMPLESREADER_H
#include <qobject.h>

class ISampleProvider : public  QObject {
    Q_OBJECT

    public:
    explicit ISampleProvider(QObject *parent = nullptr) : QObject(parent) {};
    virtual void startAcquisition(int interval) = 0;
    virtual void stopAcquisition() = 0;
    ~ISampleProvider() override = default;

    signals:
    void samplesAvailable(const QVector<double>& );
};

#endif //FEROXILLS_ISAMPLESREADER_H