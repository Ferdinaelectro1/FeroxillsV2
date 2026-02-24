//
// Created by ferdinand on 24/02/2026.
//

#include "EventBus.h"

EventBus * EventBus::getInstance() {
    static EventBus instance;
    return &instance;
}

