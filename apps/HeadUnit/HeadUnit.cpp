#include "HeadUnit.h"

HeadUnit::HeadUnit(QObject* parent)
    : QObject(parent)
{}

const QString& HeadUnit::currentGear() const
{
    return _currentGear;
}

const QString& HeadUnit::ambientLighting() const
{
    return _ambientLighting;
}

int HeadUnit::speed() const
{
    return _speed;
}

int HeadUnit::batteryPercentage() const
{
    return _batteryPercentage;
}

bool HeadUnit::chargingState() const
{
    return _chargingState;
}

void HeadUnit::setCurrentGear(const QString& gear)
{
    if (_currentGear != gear) {
        _currentGear = gear;
        emit currentGearChanged(gear);
    }
}

void HeadUnit::setAmbientLighting(const QString& color)
{
    if (_ambientLighting != color) {
        _ambientLighting = color;
        emit ambientLightingChanged(color);
    }
}

void HeadUnit::setSpeed(int speed)
{
    if (_speed != speed) {
        _speed = speed;
        emit speedChanged(speed);
    }
}

void HeadUnit::setBatteryPercentage(int batteryPercentage)
{
    if (_batteryPercentage != batteryPercentage) {
        _batteryPercentage = batteryPercentage;
        emit batteryPercentageChanged(batteryPercentage);
    }
}

void HeadUnit::setChargingState(bool state)
{
    if (_chargingState != state) {
        _chargingState = state;
        emit chargingStateChanged(state);
    }
}
