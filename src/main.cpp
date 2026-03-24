#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include "Backend.h"
#include "core/FSettings.h"
#include <QQuickStyle>

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
    qmlRegisterSingletonInstance<FSettings>("Feroxills.Settings",1,0,"Settings",FSettings::instance());

    Backend *backend = new Backend(&app);
    engine.rootContext()->setContextProperty("backend", backend);

    engine.loadFromModule("Feroxills", "Main");
    return app.exec();
}
