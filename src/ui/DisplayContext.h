//
// Created by ferdinand on 11/02/2026.
//

#ifndef FEROXILLS_DISPLAYCONTEXT_H
#define FEROXILLS_DISPLAYCONTEXT_H

#include <memory>

#include "DisplayMode.h"
#include "mode/Auto.h"
#include "mode/Continu.h"
#include "mode/Trigger.h"

class DisplayContext final : public QObject {
    Q_OBJECT
public:
    explicit DisplayContext(QObject *parent = nullptr,std::unique_ptr<FDisplayMode> initMode = nullptr) : QObject(parent), _current_mode(std::move(initMode))
     { }
    ~DisplayContext() override = default;

     void setCurrentMode(std::unique_ptr<FDisplayMode> new_mode) {
        _current_mode = std::move(new_mode);
    }

    Q_INVOKABLE [[nodiscard]] FDisplayMode::DisplayMode getCurrentMode() const {
         qDebug() << "Mode courant avec "<< _current_mode->getModeType();
         return  _current_mode->getModeType();
     }

     Q_INVOKABLE void setNewMode(const FDisplayMode::DisplayMode new_mode) {
         switch (new_mode) {
             case FDisplayMode::AUTO :
                 setCurrentMode(std::make_unique<AutoMode>());
                 break;
             case FDisplayMode::CONTINU :
                 setCurrentMode(std::make_unique<ContinuMode>());
                 break;
             case FDisplayMode::TRIGGER :
                 setCurrentMode(std::make_unique<TriggerMode>());
                 break;
             default:
                 qWarning() << "Assignation du nouveau mode échoué";
                 break;
         }
         qDebug() << "Nouveau mode" << new_mode;
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