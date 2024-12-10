#include "mainwindow.h"
#include "i2cexception.h"
#include "canbusexception.h"
#include "batterymonitor.h"
#include "canmanager.h"
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>
#include <iostream>
#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>
#include <QDebug>

#define INTERFACE_NAME "can0"
#define PYTHON_PATH "/home/falcon/piracer_test/venv/bin/python"
#define SCRIPT_PATH "/home/falcon/piracer_test/piracer_py/examples/shanwan_gamepad_control.py"


int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        MainWindow mainWindow;


        // Start and execute the Python script using QProcess
        QProcess process(&app);
        process.start(PYTHON_PATH, QStringList() << SCRIPT_PATH);
        QObject::connect(&app, &QApplication::aboutToQuit, [&process]() {
            if (process.state() == QProcess::Running) {
                process.terminate();
                if (!process.waitForFinished(3000)) {
                    process.kill();
                }
            }
        });


        // Start battery monitoring
        BatteryMonitor monitor("/dev/i2c-1", 0x41, mainWindow.battery);


        // Set the CAN BUS
        qRegisterMetaType<QCanBusFrame>("QCanBusFrame");
        CANManager canManager(INTERFACE_NAME);
        canManager.start();

        QObject::connect(&canManager, &CANManager::newMessageReceived, &mainWindow, &MainWindow::updateSpeedAnimation, Qt::QueuedConnection);


        // Set the short cut
        QShortcut ctrlQShortcut(QKeySequence("Ctrl+Q"), &mainWindow);
        QShortcut cmdQShortcut(QKeySequence("Meta+Q"), &mainWindow);

        QObject::connect(&ctrlQShortcut, &QShortcut::activated, &app, &QApplication::quit);
        QObject::connect(&cmdQShortcut, &QShortcut::activated, &app, &QApplication::quit);


        mainWindow.show();

        return app.exec();

    } catch (const I2CException& e) {
        std::cerr << "[I2CException] " << e.what() << std::endl;
    } catch (const CanBusException& e) {
        std::cerr << "[CanBusException] " << e.what() << std::endl;
    }
}
