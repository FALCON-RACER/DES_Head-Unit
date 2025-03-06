#include "./battery_client.hpp"

batteryClient::batteryClient(QObject *parent)
    : QObject(parent),
      app_(vsomeip::runtime::get()->create_application("battery")),
      batteryValue(0)
{
}

int batteryClient::getBatteryValue()
{
    return batteryValue;
}

bool batteryClient::init() {
    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }

    // 상태 핸들러 등록
    app_->register_state_handler(
            std::bind(&batteryClient::on_state, this, std::placeholders::_1));

    // 메시지 핸들러 등록
    app_->register_message_handler(
            vsomeip::ANY_SERVICE, BATTERY_INSTANCE_ID, vsomeip::ANY_METHOD,
            std::bind(&batteryClient::on_message, this, std::placeholders::_1));

    // 가용성 핸들러 등록
    app_->register_availability_handler(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID,
            std::bind(&batteryClient::on_availability, this,
                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 이벤트 구독
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(VEHICLE_EVENTGROUP_ID);
    
    app_->request_event(
            VEHICLE_SERVICE_ID,
            BATTERY_INSTANCE_ID,
            BATTERY_EVENT_ID,
            its_groups,
            vsomeip::event_type_e::ET_FIELD);
    
    app_->subscribe(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, VEHICLE_EVENTGROUP_ID);

    return true;
}

void batteryClient::start() {
    // 별도 스레드에서 실행
    std::thread vsomeip_thread([this]() {
        app_->start();
    });
    vsomeip_thread.detach(); // 백그라운드 실행
}

// void batteryClient::start() {
//     app_->start();
// }

void batteryClient::stop() {
    app_->clear_all_handler();
    app_->unsubscribe(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, VEHICLE_EVENTGROUP_ID);
    app_->release_event(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID);
    app_->release_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    app_->stop();
}

void batteryClient::on_state(vsomeip::state_type_e _state) {
    if (_state == vsomeip::state_type_e::ST_REGISTERED) {
        app_->request_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    }
}

void batteryClient::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
              << std::hex << std::setfill('0') << std::setw(4) << _service << "."
              << std::setw(4) << _instance << "] is "
              << (_is_available ? "available." : "NOT available.") << std::endl;
}

void batteryClient::on_message(const std::shared_ptr<vsomeip::message>& _response) {
    std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int*>(payload->get_data());
        std::cout << "SERVER: Received int: " << received_value << std::endl;
        if (this->batteryValue != received_value)
        {
            this->batteryValue = received_value;
            emit batteryValueChanged(this->batteryValue);
        }
    } else {
        std::cerr << "SERVER: Invalid payload size!" << std::endl;
        return;
    }

}
