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
#include "sample-ids.hpp"

#define SPEED 1
#define BATTERY 2
#define GEAR 3
#define AMBIENT 4

class service_sample {
public:
    service_sample(uint32_t _cycle, int _service);

    int getService();

    bool init();
    void start();
    void stop();
    
private:
    float speedData;
    uint8_t voltage;

    void offer();
    void stop_offer();
    void run();
    void notify();
    void on_state(vsomeip::state_type_e _state);
    void on_get(const std::shared_ptr<vsomeip::message> &_message);
    void on_set(const std::shared_ptr<vsomeip::message> &_message);
    
    void canDataReceive();
    void getBatteryVoltage();

    std::shared_ptr<vsomeip::application> app_;
    std::shared_ptr<vsomeip::payload> payload_;
    
    bool is_registered_;
    uint32_t cycle_;
    int service_;


    bool blocked_;
    bool running_;
    bool is_offered_;

    std::condition_variable condition_;
    std::condition_variable notify_condition_;
    std::condition_variable CAN_condition_; 
    std::condition_variable battery_condition_;

    std::mutex mutex_;
    std::mutex notify_mutex_;
    std::mutex payload_mutex_;
    std::mutex can_mutex_;
    std::mutex battery_mutex_;

    std::thread offer_thread_;
    std::thread notify_thread_;
    std::thread CAN_thread_;
    std::thread battery_thread_;

};

int processInit(int _service);
#endif // SERVICE_SAMPLE_HPP

