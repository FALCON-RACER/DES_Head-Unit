#include "instrumentclustercontroller.h"

InstrumentClusterController::InstrumentClusterController(QObject* parent)
    : QObject(parent)
{}

const QString& InstrumentClusterController::currentGear() const
{
    return _currentGear;
}

const QString& InstrumentClusterController::ambientLighting() const
{
    return _ambientLighting;
}

int InstrumentClusterController::speed() const
{
    return _speed;
}

int InstrumentClusterController::batteryPercentage() const
{
    return _batteryPercentage;
}

bool InstrumentClusterController::chargingState() const
{
    return _chargingState;
}

void InstrumentClusterController::setCurrentGear(const QString& gear)
{
    if (_currentGear != gear) {
        _currentGear = gear;
        emit currentGearChanged(gear);
    }
}

void InstrumentClusterController::setAmbientLighting(const QString& color)
{
    if (_ambientLighting != color) {
        _ambientLighting = color;
        emit ambientLightingChanged(color);
    }
}

void InstrumentClusterController::setSpeed(int speed)
{
    if (_speed != speed) {
        _speed = speed;
        emit speedChanged(speed);
    }
}

void InstrumentClusterController::setBatteryPercentage(int batteryPercentage)
{
    if (_batteryPercentage != batteryPercentage) {
        _batteryPercentage = batteryPercentage;
        emit batteryPercentageChanged(batteryPercentage);
    }
}

void InstrumentClusterController::setChargingState(bool state)
{
    if (_chargingState != state) {
        _chargingState = state;
        emit chargingStateChanged(state);
    }
}
