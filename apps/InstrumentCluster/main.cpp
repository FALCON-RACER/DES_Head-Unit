#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

// for test
#include <QTimer>

#include "instrumentclustercontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    InstrumentClusterController controller;

    engine.rootContext()->setContextProperty("instrumentClusterController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("InstrumentCluster", "Main");

    qDebug() << "Instrument Cluster launched";

    // test code
    QTimer *timer = new QTimer(&controller);
    QObject::connect(timer, &QTimer::timeout, [&controller]() {
        static QStringList gears = {"P", "R", "N", "D"};
        static int gearIndex = 0;
        controller.setCurrentGear(gears[gearIndex]);
        gearIndex = (gearIndex + 1) % gears.size();

        static int speed = 0;
        speed = (speed + 10) % 300;
        controller.setSpeed(speed);

        static int batteryPercentage = 100;
        batteryPercentage = (batteryPercentage - 10) < 0 ? 100 : batteryPercentage - 10;
        controller.setBatteryPercentage(batteryPercentage);

        static int chargingState = false;
        chargingState = !chargingState;
        controller.setChargingState(chargingState);

        static QStringList colors = {"#000000", "#550000", "#005500", "#000055"};
        static int colorIndex = 0;
        controller.setAmbientLighting(colors[colorIndex]);
        colorIndex = (colorIndex + 1) % colors.size();
    });
    timer->start(1000);


    return app.exec();
}
