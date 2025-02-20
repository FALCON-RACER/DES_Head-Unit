#include "./client-example.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>


// 내부 이식을 하게되면 배터리 연결체랑 속도 연결체가 나뉘게됨.
// 파트가 나뉘어진다는거지.. 
// 서버는 일단 켜지면 속도와 배터리 데이터를 받아오는 프로세스를 실행하게됨.
// 서버에서는 notify 되는 부분에서 제공을 하니...


// 이벤트 그룹도 속도 / 배터리로 분할.
// 
client_sample::client_sample(bool _use_tcp) :
        app_(vsomeip::runtime::get()->create_application("battery")), use_tcp_(_use_tcp) {
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
            vsomeip::ANY_SERVICE, BATTERY_INSTANCE_ID, vsomeip::ANY_METHOD,
            std::bind(&client_sample::on_message, this, std::placeholders::_1));

    // 가용성 핸들러 등록
    app_->register_availability_handler(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID,
            std::bind(&client_sample::on_availability, this,
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

void client_sample::start() {
    app_->start();
}

void client_sample::stop() {
    app_->clear_all_handler();
    app_->unsubscribe(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, VEHICLE_EVENTGROUP_ID);
    app_->release_event(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID);
    app_->release_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    app_->stop();
}

void client_sample::on_state(vsomeip::state_type_e _state) {
    if (_state == vsomeip::state_type_e::ST_REGISTERED) {
        app_->request_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    }
}

void client_sample::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
              << std::hex << std::setfill('0') << std::setw(4) << _service << "."
              << std::setw(4) << _instance << "] is "
              << (_is_available ? "available." : "NOT available.") << std::endl;
}

void client_sample::on_message(const std::shared_ptr<vsomeip::message>& _response) {

    std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
    if (its_payload->get_length() == sizeof(float)) { // 페이로드 크기로 검증
        float received_speed = 0.0f;

        // 이터레이터로 시작 위치 가져오기
        auto it = its_payload->get_data(); // 시작 위치의 이터레이터
        std::copy(it, it + sizeof(float), reinterpret_cast<vsomeip::byte_t *>(&received_speed));

        // 변환된 값 출력
        std::cout << "Received data: " << received_speed << " m/s" << std::endl;
    } else {
        std::cerr << "Invalid data size received!" << std::endl;
    }
}
