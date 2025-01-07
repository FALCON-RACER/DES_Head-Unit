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

class service_sample {
public:
    service_sample(uint32_t _cycle);
    bool init();
    void start();
    void stop();

private:
    void offer();
    void stop_offer();
    void run();
    void notify();

    void on_state(vsomeip::state_type_e _state);
    void on_get(const std::shared_ptr<vsomeip::message> &_message);
    void on_set(const std::shared_ptr<vsomeip::message> &_message);

    std::shared_ptr<vsomeip::application> app_;
    bool is_registered_;
    uint32_t cycle_;

    std::mutex mutex_;
    std::condition_variable condition_;
    bool blocked_;
    bool running_;

    std::mutex notify_mutex_;
    std::condition_variable notify_condition_;
    bool is_offered_;

    std::mutex payload_mutex_;
    std::shared_ptr<vsomeip::payload> payload_;

    std::thread offer_thread_;
    std::thread notify_thread_;
};

int processInit(int service);
#endif // SERVICE_SAMPLE_HPP

