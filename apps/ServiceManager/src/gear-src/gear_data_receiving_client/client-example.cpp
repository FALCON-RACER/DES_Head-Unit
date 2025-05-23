#include "./client-example.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

client_sample::client_sample(bool _use_tcp) :
        app_(vsomeip::runtime::get()->create_application("gear")), use_tcp_(_use_tcp) {
}

bool client_sample::init() {
    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }

    std::cout << "Client settings [protocol=" << (use_tcp_ ? "TCP" : "UDP") << "]" << std::endl;

    // 상태 핸들러 등록
    app_->register_state_handler(
            std::bind(&client_sample::on_state, this, std::placeholders::_1));

    // 메시지 핸들러 등록
    app_->register_message_handler(
            vsomeip::ANY_SERVICE, GEAR_INSTANCE_ID, vsomeip::ANY_METHOD,
            std::bind(&client_sample::on_message, this, std::placeholders::_1));

    // 가용성 핸들러 등록
    app_->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID,
            std::bind(&client_sample::on_availability, this,
                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 이벤트 구독
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(VEHICLE_EVENTGROUP_ID);
    
    app_->request_event(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            GEAR_EVENT_ID,
            its_groups,
            vsomeip::event_type_e::ET_FIELD);
    
    app_->subscribe(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, VEHICLE_EVENTGROUP_ID);

    return true;
}

void client_sample::start() {
    app_->start();
}

void client_sample::stop() {
    app_->clear_all_handler();
    app_->unsubscribe(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, VEHICLE_EVENTGROUP_ID);
    app_->release_event(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, GEAR_EVENT_ID);
    app_->release_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    app_->stop();
}

void client_sample::on_state(vsomeip::state_type_e _state) {
    if (_state == vsomeip::state_type_e::ST_REGISTERED) {
        app_->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    }
}

void client_sample::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
              << std::hex << std::setfill('0') << std::setw(4) << _service << "."
              << std::setw(4) << _instance << "] is "
              << (_is_available ? "available." : "NOT available.") << std::endl;
}

void client_sample::on_message(const std::shared_ptr<vsomeip::message> &_request) {
    std::shared_ptr<vsomeip::payload> payload = _request->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int*>(payload->get_data());
        std::cout << "GEAR DATA RECEIVING CLIENT : Received int: " << received_value << std::endl;
    } else {
        std::cerr << "GEAR DATA RECEIVING CLIENT : Invalid payload size!" << std::endl;
        return;
    }
    // this->gearValue = received_value;
}
