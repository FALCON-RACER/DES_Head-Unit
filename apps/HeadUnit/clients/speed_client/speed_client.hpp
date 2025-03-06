#ifndef SPEED_CLIENT_HPP
#define SPEED_CLIENT_HPP

#include <QObject>

#include "../headers.hpp"
#include "../server.hpp"

class SpeedClient : public QObject
{
    Q_OBJECT

public:
    explicit SpeedClient(QObject *parent = nullptr);

    bool init();
    void start();
    void stop();

signals:
    void speedValueChanged(float newSpeedValue);

private:
    float speedValue;

    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
};

#endif // speedClient_HPP
