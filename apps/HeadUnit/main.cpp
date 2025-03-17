#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>
#include <QBluetoothLocalDevice>
#include "modules/spotify/spotify.h"
#include "shared/utils/someip.h"
// #include "shared/utils/envmanager.h"
#include "HeadUnit.h"

#include <QTimer>
#include <iostream>
#include <vsomeip/vsomeip.hpp>
#include "./clients/speed_client/speed_client.hpp"
#include "./clients/battery_client/battery_client.hpp"
#include "./clients/gear_data_receiving_client/gear_client.hpp"
#include "./clients/ambient_sender/alsender.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    HeadUnit controller;

    // todo fix the location
    // EnvManager::instance().loadEnvFile("/home/wonjeong/head-unit/apps/HeadUnit/.env");


    // Register the Spotify class to be used in QML
    // qmlRegisterType<Spotify>("com.spotify", 1, 0, "Spotify");

    Spotify spotify;
    engine.rootContext()->setContextProperty("spotify", &spotify);

    BatteryClient batteryClient;
    engine.rootContext()->setContextProperty("batteryClient", &batteryClient);

    GearClient gearClient;
    engine.rootContext()->setContextProperty("gearClient", &gearClient);

    SpeedClient speedClient;
    engine.rootContext()->setContextProperty("speedClient", &speedClient);

    if (batteryClient.init())
        batteryClient.start();
    if (speedClient.init())
        speedClient.start();
    if (gearClient.init())
        gearClient.start();

    SomeIP someIP;
    engine.rootContext()->setContextProperty("someIP", &someIP);
 
    someIP.init();

    Alsender alClient;
    engine.rootContext()->setContextProperty("alClient", &alClient);
    
    alClient.init();
    
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [&controller, &speedClient,&batteryClient,&gearClient]() { QCoreApplication::exit(-1);
            
    },
        Qt::QueuedConnection);
    
    // engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    engine.loadFromModule("HeadUnit", "Main");

    engine.rootContext()->setContextProperty("HeadUnit", &controller);


    qDebug() << "Head Unit launched";

    return app.exec();
}
