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
#include "./clients/speed_client/speed_client.hpp"
#include "./clients/battery_client/battery_client.hpp"
#include "./clients/gear_data_receiving_client/gear_client.hpp"


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

    SomeIP someIP;
    engine.rootContext()->setContextProperty("someIP", &someIP);

    batteryClient batteryClient;
    engine.rootContext()->setContextProperty("batteryClient", &batteryClient);

    speedClient speedClient;
    gearClient gearClient;
    // alClient alClient;

    if (batteryClient.init())
        batteryClient.start();
    if (speedClient.init())
        speedClient.start();
    if (gearClient.init())
        gearClient.start();
    // QObject::connect(
    //     &engine,
    //     &QQmlApplicationEngine::objectCreationFailed,
    //     &app,
    //     []() { QCoreApplication::exit(-1); },
    //     Qt::QueuedConnection);
    
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [&controller, &speedClient,&batteryClient,&gearClient]() { QCoreApplication::exit(-1);
            static int speed = 0;
            speed = speedClient.speedValue;
            controller.setSpeed(speed);
    
            //Battery
            static int batteryPercentage = 100;
            batteryPercentage = batteryClient.batteryValue;
            controller.setBatteryPercentage(batteryPercentage);
    
            static int chargingState = false;
            chargingState = !chargingState;
            controller.setChargingState(chargingState);
    
            //Gear
            static QStringList gears = {"P", "R", "N", "D"};
            static int gearIndex = 0;
            gearIndex = gearClient.gearValue; // Drive = 3, NEUTRAL=2, REVERSE=1, PARKING=0
    
            controller.setCurrentGear(gears[gearIndex]);
            gearIndex = (gearIndex + 1) % gears.size();
    
            //Ambient Light
            // static QStringList colors = {"#4deeea", "#74ee15", "#ffe700", "#f000ff", "#001eff"};
    //         static int colorIndex = 0;
    //         colorIndex = alClient.alValue; // <- set colorIndex to received value
    
    //         controller.setAmbientLighting(colors[colorIndex]);
    //         colorIndex = (colorIndex + 1) % colors.size();
    },
        Qt::QueuedConnection);
    
    // engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    engine.loadFromModule("HeadUnit", "Main");

    engine.rootContext()->setContextProperty("HeadUnit", &controller);

    // test code
    QTimer *timer = new QTimer(&controller);
    QObject::connect(timer, &QTimer::timeout, [&controller]() {
        // static QStringList gears = {"P", "R", "N", "D"};
        // static int gearIndex = 0;
        // controller.setCurrentGear(gears[gearIndex]);
        // gearIndex = (gearIndex + 1) % gears.size();

        static int speed = 0;
        speed = (speed + 10) % 310;
        controller.setSpeed(speed);

        static int batteryPercentage = 100;
        batteryPercentage = (batteryPercentage - 10) < 0 ? 100 : batteryPercentage - 10;
        controller.setBatteryPercentage(batteryPercentage);

        static int chargingState = false;
        chargingState = !chargingState;
        controller.setChargingState(chargingState);
        
        QVector<int> colors = {20, 157, 100, 300, 200};
        static int colorIndex = 0;

        // colorIndex = alClient.alValue;
        controller.setAmbientLighting(colors[colorIndex]);
        colorIndex = (colorIndex + 1) % colors.size();
        std::cout << controller.ambientLighting() << std::endl;
    });
    timer->start(1000);

    qDebug() << "Head Unit launched";

    return app.exec();
}
