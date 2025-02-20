#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

// for test
#include <QTimer>

#include "instrumentclustercontroller.h"
//TODO: 2025-02-20. For ambient light, send/recive data type change required.
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    InstrumentClusterController controller;
    
    speedClient speedClient();
    batteryClient batteryClient();
    gearClient gearClient();
    alClient alClient();

    engine.rootContext()->setContextProperty("instrumentClusterController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("InstrumentCluster", "Main");

    qDebug() << "Instrument Cluster launched";

    if (speedClient.init())
        speedClient.start();
    if (batteryClient.init())
        batteryClient.start();
    if (gearClient.init())
        gearClient.start();
    
    alClient.start();

    // test code
    QTimer *timer = new QTimer(&controller);
    QObject::connect(timer, &QTimer::timeout, [&controller]() {
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

        //Ambient Light
        // static QStringList colors = {"#4deeea", "#74ee15", "#ffe700", "#f000ff", "#001eff"};
        static int colorIndex = 0;
        colorIndex = alClient.alValue; // <- set colorIndex to received value

        controller.setAmbientLighting(colors[colorIndex]);
        colorIndex = (colorIndex + 1) % colors.size();
    });
    timer->start(1000);


    return app.exec();
}
