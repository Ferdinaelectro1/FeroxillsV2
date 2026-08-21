#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include "Backend.h"
#include "core/FSettings.h"
#include <QQuickStyle>
#include "ui/FViewModel.h"
#include "src/core/SignalType.h"
#include "src/core/debug/debug.h"
#include "src/io/SerialPortUtils.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qInstallMessageHandler(debugMessageHandler);

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
    qmlRegisterUncreatableType<ProviderType>("Feroxills.ProviderType", 1, 0, "ProviderType", "Enum only");
    qmlRegisterSingletonInstance<FSettings>("Feroxills.Settings",1,0,"Settings",FSettings::instance());
    qmlRegisterSingletonType<SerialPortUtils>("Feroxills.SerialPortUtils", 1, 0, "SerialPortUtils",
    [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new SerialPortUtils();
    });

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
