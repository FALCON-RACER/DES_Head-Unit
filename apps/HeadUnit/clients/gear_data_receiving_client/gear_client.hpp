#ifndef GEAR_CLIENT_HPP
#define GEAR_CLIENT_HPP

#include "../headers.hpp"
#include "../server.hpp"

class gearClient {
public:
    gearClient();

    bool init();
    void start();
    void stop();
    int gearValue;

private:
    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
};

#endif // GEAR_CLIENT_HPP
