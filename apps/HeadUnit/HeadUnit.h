#pragma once

#include <QObject>
#include "./clients/speed_client/speed_client.hpp"
#include "./clients/battery_client/battery_client.hpp"
#include "./clients/gear_data_receiving_client/gear_client.hpp"
#include "./clients/ambient_receiver/al_receiver.hpp"

class HeadUnit : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentGear READ currentGear WRITE setCurrentGear NOTIFY currentGearChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int batteryPercentage READ batteryPercentage WRITE setBatteryPercentage NOTIFY batteryPercentageChanged)
    Q_PROPERTY(int chargingState READ chargingState WRITE setChargingState NOTIFY chargingStateChanged)

public:
    explicit HeadUnit(QObject *parent = nullptr);

    const QString&	currentGear() const;
    int				speed() const;
    int				batteryPercentage() const;
    bool			chargingState() const;

    void	setCurrentGear(const QString& gear);
    void	setSpeed(int speed);
    void	setBatteryPercentage(int batteryPercentage);
    void	setChargingState(bool state);

signals:
    void	currentGearChanged(const QString& gear);
    void	speedChanged(int speed);
    void	batteryPercentageChanged(int batteryPercentage);
    void	chargingStateChanged(bool state);

private:
    QString	_currentGear = "P";
    int		_speed = 0;
    int		_batteryPercentage = 100;
    bool	_chargingState = false;
};
