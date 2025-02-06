// service_sample.hpp
#ifndef SERVICE_SAMPLE_HPP
#define SERVICE_SAMPLE_HPP

#include <vsomeip/vsomeip.hpp>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <csignal>
#include "../sample-ids.hpp"
#include "../speed/canreceiver.hpp"

#define SPEED 1
#define BATTERY 2
#define GEAR 3
#define AMBIENT 4

class service_sample {
public:
    service_sample();  // 기본 생성자 추가

protected:
    virtual bool init()=0;
    virtual void start()=0;
    virtual void stop()=0;
    virtual void offer()=0;
    virtual void stop_offer()=0;
    virtual void run()=0;
    virtual void notify()=0;
    virtual void on_state(vsomeip::state_type_e _state)=0;
    virtual void on_get(const std::shared_ptr<vsomeip::message> &_message)=0;
    virtual void on_set(const std::shared_ptr<vsomeip::message> &_message)=0;

    // virtual int processInit()=0;
    
private:
};

// int processInit(int _service);
int speedProcessInit();
int batteryProcessInit();

#endif // SERVICE_SAMPLE_HPP

