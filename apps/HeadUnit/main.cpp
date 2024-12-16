#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

#include "src/spotify.h"
#include "src/envmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // todo fix the location
    EnvManager::instance().loadEnvFile("/Users/LWJ/subject/sea-me/DES_Head-Unit/apps/HeadUnit/.env");


    // Register the Spotify class to be used in QML
    // qmlRegisterType<Spotify>("com.spotify", 1, 0, "Spotify");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/resources/qml/Main.qml")));

    Spotify spotify;
    engine.rootContext()->setContextProperty("spotify", &spotify);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("HeadUnit", "Main");

    qDebug() << "Head Unit launched";

    return app.exec();
}
