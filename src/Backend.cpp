//
// Created by ferdinand on 08/02/2026.
//

#include "Backend.h"
#include "core/FConstantes.h"
#include "core/FSettings.h"
#include "core/event/EventBus.h"
#include "io/SoftwareProvider.h"

static void printSamplesParameter(const SamplesParameter& param) {
    constexpr double ech_freq = (1.0/44100.0);
    qDebug() << "*******Parameters*******";
    qDebug() << "**Voltage Max = "<< param.voltage_max;
    qDebug() << "**Voltage Min = "<< param.voltage_min;
    qDebug() << "**First rising pos = "<<param._first_rising_pos;
    qDebug() << "**Second rising pos = "<<param._second_rising_pos;
    qDebug() << "**Period Samples = " << param.periodSamples;
    qDebug() << "**Period = "<<param.periodSamples * ech_freq;
    qDebug() << "**Frequency = "<<1/(param.periodSamples * ech_freq);
}

Backend::Backend(QObject *parent) : QObject(parent),_maxVoltage(0),_display_context(this,std::make_unique<ContinuMode>()) {
    qDebug() << "App launch";
    auto s = SoftwareProviderSettings();
    s.set_interval_ms(50);
    s.set_frequency(150);
    s.set_voltage(4);
    s.set_phase(0);
    s.set_type(SignalType::SINUS);
    _source_controller = new ProviderSourceController(ProviderType::SOFTWARE_SOURCE,&s,this);
    _sample_needed = FSettings::instance()->getSamplesNeeded();
    _displaySamples.resize(_sample_needed, 0.0);
    connect(_source_controller,&ProviderSourceController::samplesAvailable,this,&Backend::dataAvailable);
    connect(FSettings::instance(),&FSettings::onTimeDivChanged,this,&Backend::onTimeDivChanged);
    /*Re-emit the signal coming from the event bus in the backend so the trigger parameters can be retrieved from QML*/
    connect(EventBus::getInstance(),&EventBus::TriggerModeDisplayInvoked,this,&Backend::triggerModeDisplayInvoked);
    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&Backend::onTimeOut); //timer d'affichage de chaque frame (on peut regler le fps ici)
    _timer->setInterval(50);
    _timer->start();
    connect(_source_controller, &ProviderSourceController::currentProviderTypeChanged,
    this, [this](ProviderType::Type) {
        // Reset the display buffer on provider switch, otherwise the
        // previous provider's last frame stays frozen on screen until
        // the new provider emits its first samples.
        _samplesRingBuf.fillAllWith(0);
});
}

double Backend::getMaxVoltage() const {
    return  _maxVoltage;
}

void Backend::setMaxVoltage(const QVector<double>& voltageSamples) {
    for (const double voltage :  voltageSamples) {
        if (voltage > _maxVoltage) {
            _maxVoltage = voltage;
            emit MaxVoltageChanged();
        }
    }
}

bool Backend::get_run() const {
    return  _run;
}

void Backend::setRun(const bool run) {
    _run = run;
}

void Backend::dataAvailable(const QVector<double>& data) {
    _samples = data.toVector();

    for (const double ech : _samples)
        _samplesRingBuf.push(ech);

    setMaxVoltage(_samples);
}

void Backend::onTimeDivChanged() {
    _sample_needed = static_cast<unsigned long>((FSettings::instance()->getTimeDiv() * Feroxills::Constants::HORIZONTAL_DIVISIONS) / Feroxills::Constants::SAMPLING_PERIOD);
    FSettings::instance()->setSamplesNeeded(_sample_needed);
    _displaySamples.resize(_sample_needed, 0.0);
    const  double intervalOfAcquisition = Feroxills::Constants::SAMPLING_PERIOD * static_cast<double>(_sample_needed);
    qDebug() << "Samples need = " << _sample_needed << " et intervalOfAcquisition = " << intervalOfAcquisition*1000 << " et TimeDiv = "<<FSettings::instance()->getTimeDiv();
    if (auto *sw = dynamic_cast<SoftwareProviderSettings *>(_source_controller->getCurrentProviderSettings())) {
        sw->set_interval_ms(static_cast<int>(intervalOfAcquisition*1000));
    } else {
        qWarning() << "We try to modify parameter on source who don't have this parameter";
    }
}

void Backend::onTimeOut() {
    static bool analyse = false;
    static unsigned long count = 0;
    if (_run) {
        count++;
        if (!analyse && count > 200) {
            const QVector<double> analyseVec = _samplesRingBuf.getRecentWindows(512);
            const SamplesParameter param = _analyser.getSamplesParameter(analyseVec);
            if (param.isPeriodic)
              printSamplesParameter(param);
            else
                qWarning() << "This signal is not a periodic signal";
            analyse = true;
        }
        // Send the data to the display-processing system to decide how to render it
        // using the circular buffer
        _display_context.processDisplaySamples(&_samplesRingBuf,_displaySamples.data(),_sample_needed);
        emit  displaySamplesReady(_displaySamples);
    }
}


ProviderSourceController * Backend::getProviderSourceController() const {
    return  _source_controller;
}