#include "client-example.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

client_sample::client_sample(bool _use_tcp) :
        app_(vsomeip::runtime::get()->create_application()), use_tcp_(_use_tcp) {
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
            vsomeip::ANY_SERVICE, SAMPLE_INSTANCE_ID, vsomeip::ANY_METHOD,
            std::bind(&client_sample::on_message, this, std::placeholders::_1));

    // 가용성 핸들러 등록
    app_->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID,
            std::bind(&client_sample::on_availability, this,
                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 이벤트 구독
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(SAMPLE_EVENTGROUP_ID);
    app_->request_event(
            SAMPLE_SERVICE_ID,
            SAMPLE_INSTANCE_ID,
            SAMPLE_EVENT_ID,
            its_groups,
            vsomeip::event_type_e::ET_FIELD);
    app_->subscribe(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENTGROUP_ID);

    return true;
}

void client_sample::start() {
    app_->start();
}

void client_sample::stop() {
    app_->clear_all_handler();
    app_->unsubscribe(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENTGROUP_ID);
    app_->release_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID);
    app_->release_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app_->stop();
}

void client_sample::on_state(vsomeip::state_type_e _state) {
    if (_state == vsomeip::state_type_e::ST_REGISTERED) {
        app_->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    }
}

void client_sample::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
              << std::hex << std::setfill('0') << std::setw(4) << _service << "."
              << std::setw(4) << _instance << "] is "
              << (_is_available ? "available." : "NOT available.") << std::endl;
}

void client_sample::on_message(const std::shared_ptr<vsomeip::message>& _response) {
    std::stringstream its_message;
    its_message << "Received a notification for Event ["
                << std::hex << std::setfill('0')
                << std::setw(4) << _response->get_service() << "."
                << std::setw(4) << _response->get_instance() << "."
                << std::setw(4) << _response->get_method() << "] to Client/Session ["
                << std::setw(4) << _response->get_client() << "/"
                << std::setw(4) << _response->get_session() << "] = ";

    std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
    its_message << "(" << std::dec << its_payload->get_length() << ") ";

    for (uint32_t i = 0; i < its_payload->get_length(); ++i) {
        its_message << std::hex << std::setw(2) << static_cast<int>(its_payload->get_data()[i]) << " ";
    }
    std::cout << its_message.str() << std::endl;
}
