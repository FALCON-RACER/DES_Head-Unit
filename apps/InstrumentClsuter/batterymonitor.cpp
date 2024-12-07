#include "batterymonitor.h"
#include <QDebug>

/**
 * @brief Constructor for BatteryMonitor
 * @param i2cDevice     The I2C device path
 * @param address       The I2C address of the INA219 sensor
 * @param progressBar   The widget of battery GUI
 * @param parent        The parent QObject, if any
 */
BatteryMonitor::BatteryMonitor(const QString& i2cDevice, uint8_t address, QProgressBar *progressBar, QObject *parent)
    : QObject(parent)
    , ina219(new INA219(i2cDevice.toStdString().c_str(), address))
    , progressBar(progressBar)
    , lowBatteryVoltage(9.5)
{
    updateBatteryVoltage();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BatteryMonitor::updateBatteryVoltage);
    timer->start(60000);    // send battery data every 1 min (1000ms = 1 sec)
}

BatteryMonitor::~BatteryMonitor() {
    if (ina219) {
        delete ina219;
        ina219 = nullptr;
    }
    if (timer) {
        delete timer;
        timer = nullptr;
    }
    if (progressBar) {
        delete progressBar;
        progressBar = nullptr;
    }
}


/**
 * @brief Updates the GUI's battery data
 *
 * It turns off when the bus voltage is 8.4V
 * We set the low voltage 9.5V
 * Full charge : 12.45
 *
 *
 * 12.4 - 9.5 = 2.9 = 100%
 * 2.9 / 100 = 0.029 = 1%
 * difference between current value and lowest value divided by 1% value gives current %
 */
void BatteryMonitor::updateBatteryVoltage() {

    float voltage = ina219->getBatteryVoltage();
    float voltagePercent = (voltage - lowBatteryVoltage) / 0.029;

    if (voltagePercent >= 100)
        voltagePercent = 100;
    else if (voltagePercent <= 0)
        voltagePercent = 0;


    progressBar->setValue(voltagePercent);

    qDebug() << "Battery Voltage:" << voltage << "V";
}
