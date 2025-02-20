#ifndef CLIENT_SAMPLE_HPP
#define CLIENT_SAMPLE_HPP

#include <vsomeip/vsomeip.hpp>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <csignal>
#include "server.hpp"

class client_sample {
public:
    client_sample(bool _use_tcp);

    bool init();
    void start();
    void stop();

private:
    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
    bool use_tcp_;
};

#endif // CLIENT_SAMPLE_HPP
