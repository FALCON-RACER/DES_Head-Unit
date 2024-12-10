#include "../../../header/IC/batterychecker.h"
#include <QDebug>

BatteryChecker::BatteryChecker(QObject *parent)
    : QObject(parent)
    , busVoltage(0)
    , shuntVoltage(0)
    , amper(0)
    , isCharging(false)
    , busVoltagePercent(0)
{
    this->ina219Reader = new INA219Reader(); // Initializing the object for reading data via I2C
    this->voltageFilter = new SmaFilter(this, FILTER_WINDOW_SIZE); // Initializing the filter with the specified window size

    this->voltageRange = MAX_BUS_VOLTAGE - MIN_BUS_VOLTAGE; // Calculate the difference between max and min bus voltage

    this->timer = new QTimer(this); // Create a timer for periodic data reading
    connect(this->timer, &QTimer::timeout, this, &BatteryChecker::readBatteryData); // Connect the timer to the slot for reading data
}

BatteryChecker::~BatteryChecker() {
    timer->stop(); // Stop the timer before the object is destroyed
    delete ina219Reader; // Delete the I2C reader object
    delete voltageFilter; // Delete the filter object
}

void BatteryChecker::startMonitoring(int interval) {
    timer->start(interval); // Start the timer with the specified interval
}

float BatteryChecker::getBusVoltage() const {
    return this->busVoltage; // Return the current bus voltage
}

float BatteryChecker::getShuntVoltage() const {
    return this->shuntVoltage; // Return the current shunt voltage
}

float BatteryChecker::getAmper() const {
    return this->amper; // Return the current amperage
}

bool BatteryChecker::getIsCharging() const {
    return this->isCharging; // Return the current charging state
}

int BatteryChecker::getBusVoltagePercent() const {
    return busVoltagePercent; // Return the bus voltage percentage
}

void BatteryChecker::readBatteryData() {
    try {
        this->busVoltage = ina219Reader->readBusVoltage(); // Read the current bus voltage
        this->shuntVoltage = ina219Reader->readShuntVoltage(); // Read the current shunt voltage
        this->amper = ina219Reader->readAmper(); // Read the current amperage

        this->checkIsCharge(); // Check for changes in the charging state
        this->updateBusVoltagePercent(); // Update the bus voltage percentage
    } catch (const std::exception &e) {
        qCritical() << "Exception: " << e.what();
    }
}

void BatteryChecker::checkIsCharge() {
    // If the shunt voltage is greater than 40 mV but less than 400 mV, and the current is greater than 3 mA but less than 60 mA, then the battery is charging.
    bool new_isCharge = this->shuntVoltage > SHUNT_VOLTAGE_MIN_THRESHOLD
                        && this->shuntVoltage < SHUNT_VOLTAGE_MAX_THRESHOLD
                        && this->amper > AMPERAGE_MIN_THRESHOLD
                        && this->amper < AMPERAGE_MAX_THRESHOLD;

    // Check the charging condition considering the new threshold values
    if (new_isCharge != this->isCharging) { // If the charging state has changed, update and emit a signal
        this->isCharging = new_isCharge;
        emit chargingStateUpdated(this->isCharging);
    }
}

void BatteryChecker::updateBusVoltagePercent() {
    float adjustedBusVoltage = this->busVoltage; // Initial bus voltage value

    if (this->isCharging) { // If charging, reduce the bus voltage by the increment value
        adjustedBusVoltage -= CHARGE_VOLTAGE_INCREMENT;
    }

    float relativeVoltage = (adjustedBusVoltage - MIN_BUS_VOLTAGE) / this->voltageRange; // Calculate the relative voltage
    int newBusVoltagePercent = static_cast<int>(relativeVoltage * 100.0f); // Convert to percentage

    if (newBusVoltagePercent < 0) {
        newBusVoltagePercent = 0;
    } else if (newBusVoltagePercent > 100) {
        newBusVoltagePercent = 100;
    }

    this->busVoltagePercent = static_cast<int>(this->voltageFilter->calculateFilteredOutput(static_cast<double>(newBusVoltagePercent))); // Apply the filter to smooth the output
    emit batteryPercentageUpdated(this->busVoltagePercent);
}
