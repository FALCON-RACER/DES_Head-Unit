#ifndef SPEEDUPDATEMANAGER_H
#define SPEEDUPDATEMANAGER_H

#include <QObject>

#include "canreceiver.h"
#include "./filter/filtermanager.h"

class SpeedUpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit SpeedUpdateManager(QObject *parent = nullptr);
    ~SpeedUpdateManager();

    void start() const;

    FilterManager* getFilterManager() const;

signals:
    void speedUpdated(double speed);

private:
    CanReceiver* canReceiver;
    FilterManager* filterManager;

    const double SCALE_FACTOR = 10000.0;

    void processSpeedData();

    class CanDeviceConnectionFailed : public std::exception
    {
    public:
        const char* what() const throw();
    };
};

#endif // SPEEDUPDATEMANAGER_H
