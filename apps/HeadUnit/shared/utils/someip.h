#ifndef SOMEIP_H
#define SOMEIP_H

#include "./server.hpp"

#include <QObject>
#include <QDebug>

#include <vsomeip/vsomeip.hpp>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <csignal>
#include <iostream>
class SomeIP : public QObject
{
	Q_OBJECT
public:
	explicit SomeIP(QObject *parent = nullptr) : QObject(parent) {}

	Q_INVOKABLE void send_gear_data(int);
};

#endif // SOMEIP_H
