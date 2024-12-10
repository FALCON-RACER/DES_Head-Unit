#include "../../../header/IC/speedupdatemanager.h"

#include <QCanBusDevice>
#include <QDataStream>
#include <QDebug>

SpeedUpdateManager::SpeedUpdateManager(QObject *parent)
    : QObject{parent}
    , canReceiver(new CanReceiver(this, 0x100, "can1"))
    , filterManager(new FilterManager(this))
{
    this->canReceiver->createCanDevice();
}

SpeedUpdateManager::~SpeedUpdateManager()
{}

void SpeedUpdateManager::start() const
{
    this->connect(this->canReceiver->getCanDevice()
              , &QCanBusDevice::framesReceived
              , this
              , &SpeedUpdateManager::processSpeedData);
    if (!this->canReceiver->getCanDevice()->connectDevice())
    {
        throw CanDeviceConnectionFailed();
    }
}

FilterManager* SpeedUpdateManager::getFilterManager() const
{
    return this->filterManager;
}

void SpeedUpdateManager::processSpeedData()
{
    this->canReceiver->processReceivedFrames();

    while (!this->canReceiver->isPayloadQueueEmpty())
    {
        QByteArray payload = this->canReceiver->getFrontPayload();

        if (payload.size() == sizeof(uint32_t))
        {
            unsigned int scaledSpeed = 0;

            QDataStream stream(payload);
            stream.setByteOrder(QDataStream::LittleEndian);
            stream >> scaledSpeed;

            double speed = this->filterManager->filter(static_cast<double>(scaledSpeed) / this->SCALE_FACTOR);
            if (speed < 1.0f)
            {
                this->filterManager->reset();
                speed = 0.0f;
            }
            emit speedUpdated(speed);
        }
    }
}

const char* SpeedUpdateManager::CanDeviceConnectionFailed::what() const throw()
{
    return "Failed to connect CAN device to slot.";
}
