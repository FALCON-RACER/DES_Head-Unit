#ifndef BATTERY_CHECKER_H
#define BATTERY_CHECKER_H

#include <QObject>
#include <QTimer>
#include "ina219reader.h"
#include "./filter/smafilter.h"

// The BatteryChecker class is designed to monitor battery parameters via I2C.

class BatteryChecker : public QObject
{
    Q_OBJECT
public:
    explicit BatteryChecker(QObject *parent = nullptr);
    ~BatteryChecker();

    void startMonitoring(int interval = DEFAULT_INTERVAL_MS); // Method to start monitoring with a specified interval

    float getBusVoltage() const;
    float getShuntVoltage() const;
    float getAmper() const;
    Q_INVOKABLE bool getIsCharging() const;
    Q_INVOKABLE int getBusVoltagePercent() const;

signals:
    void chargingStateUpdated(bool isCharging); // Signal emitted when the charging state changes
    void batteryPercentageUpdated(int busVoltagePercent);

private slots:
    void readBatteryData(); // Slot for reading battery data

private:
    void checkIsCharge(); // Method to check for changes in the charging state
    void updateBusVoltagePercent(); // Method to update the bus voltage percentage

    INA219Reader *ina219Reader; // Pointer to the object for reading data via I2C
    SmaFilter *voltageFilter; // Pointer to the filter for smoothing voltage values

    float busVoltage; // Variable for storing the current bus voltage
    float shuntVoltage; // Variable for storing the current shunt voltage
    float amper; // Variable for storing the current amperage
    bool isCharging; // Variable for storing the charging state
    int busVoltagePercent; // Variable for storing the bus voltage percentage
    float voltageRange; // Variable for storing the difference between max and min bus voltage

    QTimer *timer; // Timer for periodic data polling

    static constexpr float MIN_BUS_VOLTAGE = 9.65f; // Minimum allowable bus voltage (volts)
    static constexpr float MAX_BUS_VOLTAGE = 12.5f; // Maximum allowable bus voltage (volts)
    static constexpr float CHARGE_VOLTAGE_INCREMENT = 0.88f; // Voltage increment on the bus during charging (volts)
    static constexpr int DEFAULT_INTERVAL_MS = 1000; // Default polling interval in milliseconds
    static constexpr int FILTER_WINDOW_SIZE = 10; // Filter window size

    // Constants for determining the charging state
    static constexpr int SHUNT_VOLTAGE_MIN_THRESHOLD = 40;  // Minimum shunt voltage threshold (millivolts)
    static constexpr int SHUNT_VOLTAGE_MAX_THRESHOLD = 400; // Maximum shunt voltage threshold (millivolts)
    static constexpr int AMPERAGE_MIN_THRESHOLD = 3;  // Minimum amperage threshold (milliamps)
    static constexpr int AMPERAGE_MAX_THRESHOLD = 60; // Maximum amperage threshold (milliamps)
};

#endif // BATTERY_CHECKER_H
