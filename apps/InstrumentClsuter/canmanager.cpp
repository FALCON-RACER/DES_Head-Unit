#include "canmanager.h"
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include "canbusexception.h"


CANManager::CANManager(const QString &interfaceName, QObject *parent) : interfaceName(interfaceName), QObject(parent) {

    activateCanInterface();

    canThread = new QThread(this);
    canReceiver = new CANReceiver(interfaceName);

    canReceiver->moveToThread(canThread);

    connect(canReceiver, &CANReceiver::newMessageReceived, this, &CANManager::handleNewMessage);

    connect(canThread, &QThread::started, canReceiver, &CANReceiver::connectToBus);

    connect(canThread, &QThread::finished, canReceiver, &QObject::deleteLater);
    connect(canThread, &QThread::finished, canThread, &QObject::deleteLater);
}

CANManager::~CANManager() {

    if (canThread->isRunning()) {
        canThread->quit();
        canThread->wait();
    }

    if (canThread) {
        delete canThread;
        canThread = nullptr;
    }

    deactivateCanInterface();
}

void CANManager::start() {

    canThread->start();
}


void CANManager::handleNewMessage(const QCanBusFrame &frame) {

    emit newMessageReceived(frame);
}

void CANManager::activateCanInterface() {
    QProcess process;
    QString command = QString("ip link show %1").arg(interfaceName);

    // 인터페이스 상태 확인
    process.start(command);
    if (!process.waitForFinished())
        qWarning() << "Failed to execute command:" << process.errorString();

    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (error.isEmpty() && output.contains("state UP")) {
        qDebug() << "CAN interface" << interfaceName << "is already up.";
        return;
    }

    // 인터페이스가 비활성화된 경우, 활성화 명령어 실행
    command = QString("sudo ip link set %1 up type can bitrate 500000").arg(interfaceName);
    process.start(command);
    if (!process.waitForFinished())
        qWarning() << "Failed to execute command:" << process.errorString();

    // 명령어 출력 확인
    output = process.readAllStandardOutput();
    error = process.readAllStandardError();

    if (!error.isEmpty())
        throw CanBusException("Error activating CAN interface:") ;
    else
        qDebug() << "CAN interface activated successfully ! " << output;
}

void CANManager::deactivateCanInterface() {

    QProcess process;
    QString command = QString("sudo ip link set %1 down").arg(interfaceName);

    process.start(command);
    if (!process.waitForFinished())
        qWarning() << "Failed to execute command:" << process.errorString();
    else
        qDebug() << interfaceName << "interface closed";
}
