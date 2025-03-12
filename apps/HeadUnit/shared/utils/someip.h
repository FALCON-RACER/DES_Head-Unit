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
	// SomeIP();
	std::shared_ptr<vsomeip::application> app;
	bool init();
	void start();
	
    explicit SomeIP(QObject *parent = nullptr) : QObject(parent), app(vsomeip::runtime::get()->create_application("gear")){
	}
	Q_INVOKABLE void set_gear_data(int gearValue);	
	// Q_INVOKABLE void send_gear_data(int);

};

#endif // SOMEIP_H
