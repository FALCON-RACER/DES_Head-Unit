#ifndef AL_RECEIVER_HPP
# define AL_RECEIVER_HPP

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

class AlClient : public QObject
{
  Q_OBJECT  
public :
  // AlClient(void);
  // ~AlClient(void);
  int alValue;
  std::shared_ptr<vsomeip::application> app;
	bool init();
	void start();
  void on_message(const std::shared_ptr<vsomeip::message> &_request);
  explicit AlClient(QObject *parent = nullptr) : QObject(parent), app(vsomeip::runtime::get()->create_application("ambient")){
	}
  // Q_INVOKABLE void set_al_data(int);
};

#endif
