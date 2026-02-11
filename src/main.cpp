#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include "Backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterUncreatableType<FDisplayMode>("Feroxills.DisplayMode", 1, 0, "FDisplayMode", "Enum only");


    Backend *backend = new Backend(&app);
    engine.rootContext()->setContextProperty("backend", backend);

    engine.loadFromModule("Feroxills", "Main");
    return app.exec();
}
