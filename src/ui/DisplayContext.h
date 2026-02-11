//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_DISPLAYCONTEXT_H
#define FEROXILLS_DISPLAYCONTEXT_H

#include <memory>

#include "DisplayMode.h"

class DisplayContext {

public:
    DisplayContext(std::unique_ptr<FDisplayMode> initMode = nullptr) : _current_mode(std::move(initMode)) {}
    ~DisplayContext() = default;

    void setCurrentMode(std::unique_ptr<FDisplayMode> new_mode) {
        _current_mode = std::move(new_mode);
    }

    void processDisplaySamples(double *displaySamples, const size_t size) const {
        if (_current_mode) {
            _current_mode->processDisplaySamples(displaySamples, size);
        }
        else {
            qWarning() << "Aucun système de traitement trouvé";
        }
    }

private:
    std::unique_ptr<FDisplayMode> _current_mode;
};

#endif //FEROXILLS_DISPLAYCONTEXT_H