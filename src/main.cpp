#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include "Backend.h"
#include "core/FSettings.h"
#include <QQuickStyle>
#include "ui/FViewModel.h"
#include "src/core/SignalType.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterUncreatableType<FDisplayMode>("Feroxills.DisplayMode", 1, 0, "FDisplayMode", "Enum only");
    qmlRegisterUncreatableType<SignalType>("Feroxills.SignalType", 1, 0, "SignalType", "Enum only");
    qmlRegisterSingletonInstance<FSettings>("Feroxills.Settings",1,0,"Settings",FSettings::instance());

    Backend *backend = new Backend(&app);
    FViewModel viewModel(FSettings::instance()->getCh1VoltDiv(),&app);
    QObject::connect(backend, &Backend::displaySamplesReady, &viewModel, &FViewModel::setSamples);
    QObject::connect(FSettings::instance(),&FSettings::onCh1VoltDivChanged, &viewModel, [&]() {
        viewModel.setVerticalScales(FSettings::instance()->getCh1VoltDiv());
    });
    engine.rootContext()->setContextProperty("viewModel", &viewModel);
    engine.rootContext()->setContextProperty("backend", backend);

    engine.loadFromModule("Feroxills", "Main");
    return app.exec();
}
