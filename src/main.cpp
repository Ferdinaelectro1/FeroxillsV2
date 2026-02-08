#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QVector>
#include "src/core/signalgenerator.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<double> samples READ getSamples NOTIFY samplesChanged)
public:
    explicit Backend(QObject *parent = nullptr) : QObject(parent){}
    qreal someValue();

signals:
    void valueChanged();
public slots:
    void dataAvailable(const QVector<double>& data)
    {
        if(data.size() < 50) {
            qDebug() << "Taille de data reçu : "<<data.size();
            return;
        }
        qDebug() << "Taille de data reçu : "<<data.size();
        for(int i = 0; i< 50;i++)
        {
            qDebug() << "ech n° "<<i<<" = "<<data[i];
        }
    }
};

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

    engine.loadFromModule("Feroxills", "Main");
    Backend *backend = new Backend(&app);
    engine.rootContext()->setContextProperty("backend", backend);

    return app.exec();
}
