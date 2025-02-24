#ifndef SPEED_CLIENT_HPP
#define SPEED_CLIENT_HPP

#include "../headers.hpp"
#include "../server.hpp"

class speedClient{
public:
    speedClient();

    bool init();
    void start();
    void stop();

signals:
    void speedReceived(int speed);

private:
    float speedValue;
    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
};

#endif // speedClient_HPP
