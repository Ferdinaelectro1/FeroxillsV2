//
// Created by ferdinand on 24/02/2026.
//

#ifndef FEROXILLS_EVENTBUS_H
#define FEROXILLS_EVENTBUS_H
#include <qobject.h>

class EventBus final : public  QObject {
    Q_OBJECT
public:
    static EventBus *getInstance();
    /*Suppression des constructeurs par défaut (classe Singleton)*/
    EventBus(const EventBus &) = delete;
    EventBus(EventBus &&) = delete;
    EventBus &operator=(const EventBus &) = delete;
    EventBus &operator=(EventBus &&) = delete;

signals:
    void TriggerModeDisplayInvoked(QObject *triggerDisplayMode);

private:
    EventBus() = default;
};


#endif //FEROXILLS_EVENTBUS_H