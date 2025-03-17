#ifndef ALSENDER_H
#define ALSENDER_H

#include "../headers.hpp"
#include "../server.hpp"

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
class Alsender : public QObject
{
	Q_OBJECT
public:
	std::shared_ptr<vsomeip::application> app;
	bool init();
	void start();
	
	explicit Alsender(QObject *parent = nullptr) : QObject(parent), app(vsomeip::runtime::get()->create_application("ambient")){
	}
	// Q_INVOKABLE void set_gear_data(int gearValue);	
	Q_INVOKABLE void set_al_data(int);	
	// Q_INVOKABLE void send_gear_data(int);

};

#endif // SOMEIP_H
