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
    void startAcquisition(const int interval = 500) {
        doStartAcquisition(interval);
    }
    virtual void setAcquisitionInterval(int intervalle) = 0;
    void stopAcquisition() {
        doStopAcquisition();
    };
    ~ISampleProvider() override = default;

    signals:
    void samplesAvailable(const QVector<double>& );

private:
    virtual void doStartAcquisition(int interval) = 0;
    virtual void doStopAcquisition() = 0;
};

#endif //FEROXILLS_ISAMPLESREADER_H