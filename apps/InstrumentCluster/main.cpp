#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

// for test
#include <QTimer>
// #include <iostream>
#include <vsomeip/vsomeip.hpp>
#include "./clients/speed_client/speed_client.hpp"
#include "./clients/battery_client/battery_client.hpp"
#include "./clients/gear_data_receiving_client/gear_client.hpp"
#include "./clients/ambient_receiver/al_receiver.hpp"


#include "instrumentclustercontroller.h"
//TODO: 2025-02-20. For ambient light, send/recive data type change required.
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    InstrumentClusterController controller;
    engine.rootContext()->setContextProperty("instrumentClusterController", &controller);

    BatteryClient batteryClient;
    engine.rootContext()->setContextProperty("batteryClient", &batteryClient);

    GearClient gearClient;
    engine.rootContext()->setContextProperty("gearClient", &gearClient);

    SpeedClient speedClient;
    engine.rootContext()->setContextProperty("speedClient", &speedClient);

    AlClient ambientClient;
    engine.rootContext()->setContextProperty("ambientClient", &ambientClient);

    if (batteryClient.init())
        batteryClient.start();
    if (speedClient.init())
        speedClient.start();
    if (gearClient.init())
        gearClient.start();
    if (ambientClient.init())
        ambientClient.start();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("InstrumentCluster", "Main");

    qDebug() << "Instrument Cluster launched";
    
    // alClient.start();

    // test code
    QTimer *timer = new QTimer(&controller);
    QObject::connect(timer, &QTimer::timeout, [&controller, &speedClient,&batteryClient,&gearClient, &ambientClient]() {
        //Speed
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

        // Ambient Light
        static QStringList colors = {"#4deeea", "#74ee15", "#ffe700", "#f000ff", "#001eff"};
        static int colorIndex = 0;
        colorIndex = ambientClient.alValue; // <- set colorIndex to received value

        controller.setAmbientLighting(colors[colorIndex]);
        colorIndex = (colorIndex + 1) % colors.size();
    });
    timer->start(1000);


    return app.exec();
}
