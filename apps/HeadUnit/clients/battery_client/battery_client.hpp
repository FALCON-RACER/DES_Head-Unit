#ifndef BATTERY_CLIENTHPP
#define BATTERY_CLIENTHPP

#include <QObject>

#include "../headers.hpp"
#include "../server.hpp"

class BatteryClient : public QObejct
{
    Q_OBJECT

public:
    explicit BatteryClient(QObject *parent = nullptr);

    bool init();
    void start();
    void stop();

signals:
    void batteryValueChanged(int newBatteryValue);

private:
    int batteryValue;

    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
};

#endif // BATTERY_CLIENTHPP
