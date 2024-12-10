#ifndef CANRECEIVER_HPP
#define CANRECEIVER_HPP

#include <QObject>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <queue>

class CanReceiver : public QObject
{
    Q_OBJECT

public:
    explicit CanReceiver(QObject *parent = nullptr
                         , int expectedId = 0x0
                         , const QString& interfaceName = "");
    ~CanReceiver();

    void createCanDevice();
    void processReceivedFrames();

    QCanBusDevice*	getCanDevice() const;

    QByteArray getFrontPayload();
    bool isPayloadQueueEmpty() const;

signals:
    void speedUpdated(double speed);

private:
    QCanBusDevice *canDevice;
    const int expectedId;
    const QString interfaceName;
    std::queue<QByteArray> payloadQueue;

};

#endif // CANRECEIVER_H
