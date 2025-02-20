#include "../../headers.hpp"
#include "./gear.hpp"
#include "../../server.hpp"

//TODO: Joystick input이랑 HU input이 동시에 처리가 안되게끔 처리해야함 스레드 추가?

gearObject::gearObject(uint32_t _cycle) :
    app_(vsomeip::runtime::get()->create_application("gear")),
    is_registered_(false),
    cycle_(_cycle),
    blocked_(false),
    running_(true),
    is_offered_(false),
    is_using_(false),
    offer_thread_(std::bind(&gearObject::run, this)),
    notify_thread_(std::bind(&gearObject::notify, this)),
    gearValue(0) {
    }

bool gearObject::init() {
    std::lock_guard<std::mutex> its_lock(mutex_);

    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }
    
    app_->register_state_handler(
            std::bind(&gearObject::on_state, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            GET_METHOD_ID,
            std::bind(&gearObject::on_get, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            SET_METHOD_ID,
            std::bind(&gearObject::on_set, this, std::placeholders::_1));
    
    // process Joystick, HU input data.
    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            GEAR_SET_METHOD_ID,
            std::bind(&gearObject::on_gear_set, this, std::placeholders::_1));

    // Gear method
    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            JOY_GEAR_SET_MID,
            std::bind(&gearObject::on_joy_gear_set, this, std::placeholders::_1));
    
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(VEHICLE_EVENTGROUP_ID);

    // event provide function
    app_->offer_event(
            VEHICLE_SERVICE_ID,
            GEAR_INSTANCE_ID,
            GEAR_EVENT_ID,
            its_groups,
            vsomeip::event_type_e::ET_FIELD, std::chrono::milliseconds::zero(),
            false, true, nullptr, vsomeip::reliability_type_e::RT_UNKNOWN);

    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        payload_ = vsomeip::runtime::get()->create_payload();
    }

    blocked_ = true;
    condition_.notify_one();
    return true;
}

void gearObject::start() {
    app_->start();
}

void gearObject::stop() {
    std::cout << "SERVER : stop" << std::endl;
    running_ = false;
    blocked_ = true;

    condition_.notify_one();
    notify_condition_.notify_one();
    app_->clear_all_handler();
    stop_offer();
    
    if (offer_thread_.joinable()) offer_thread_.join();
    if (notify_thread_.joinable()) notify_thread_.join();
    app_->stop();

}

void gearObject::offer() {
    std::cout << "SERVER : offer" << std::endl;
    std::lock_guard<std::mutex> its_lock(notify_mutex_);
    app_->offer_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    is_offered_ = true;
    notify_condition_.notify_one();
    std::cout << "can condition notified" << std::endl;
}

void gearObject::stop_offer() {
    std::cout << "SERVER : stop offer" << std::endl;
    app_->stop_offer_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    is_offered_ = false;
}

void gearObject::run() {
    std::unique_lock<std::mutex> its_lock(mutex_);
    while (!blocked_) condition_.wait(its_lock);

    bool is_offer = true;
    while (running_) {
        if (is_offer) offer();
        else stop_offer();

        for (int i = 0; i < 10 && running_; i++)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}

// notify to IC and HU
void gearObject::notify() {
    while (running_) {
        std::unique_lock<std::mutex> its_lock(notify_mutex_);
        while (!is_offered_ && running_) 
            notify_condition_.wait(its_lock);
        while(is_offered_ && running_) {
                    {
                        std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
                        std::vector<vsomeip::byte_t> gear_data(
                        reinterpret_cast<vsomeip::byte_t *>(&gearValue),
                        reinterpret_cast<vsomeip::byte_t *>(&gearValue) + sizeof(int));
                        {
                            std::lock_guard<std::mutex> its_lock(payload_mutex_);
                            payload_->set_data(gear_data);
                            app_->notify(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, GEAR_EVENT_ID, payload_);
                        }
                    }
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
    }
}

void gearObject::on_state(vsomeip::state_type_e _state) {
    std::cout << "Application " << app_->get_name() << " is "
              << (_state == vsomeip::state_type_e::ST_REGISTERED ?
                  "registered." : "deregistered.") << std::endl;
    is_registered_ = (_state == vsomeip::state_type_e::ST_REGISTERED);
    
}

void gearObject::on_get(const std::shared_ptr<vsomeip::message> &_message) {
    std::shared_ptr<vsomeip::message> its_response =
        vsomeip::runtime::get()->create_response(_message);
    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        its_response->set_payload(payload_);
    }
    app_->send(its_response);
}

void gearObject::on_set(const std::shared_ptr<vsomeip::message> &_message) {
    std::shared_ptr<vsomeip::message> its_response =
        vsomeip::runtime::get()->create_response(_message);
    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        payload_ = _message->get_payload();
        its_response->set_payload(payload_);
    }
    //요청에 대한 응답 전송
    app_->send(its_response);
    //이 이벤트를 구독중인 클라이언트들에게 데이터 변경사항을 알림. broadcast의 개념
    app_->notify(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, GEAR_EVENT_ID, payload_);

}

// HU input, Joystick으로부터 데이터를 받는 부분. 데이터 갱신 필요.
void gearObject::on_gear_set(const std::shared_ptr<vsomeip::message> &_request) {
    std::cout << "#### ON GEAR SET FUNCTION" << std::endl;
    std::shared_ptr<vsomeip::payload> payload = _request->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int*>(payload->get_data());
        std::cout << "SERVER: Received int: " << received_value << std::endl;
    } else {
        std::cerr << "SERVER: Invalid payload size!" << std::endl;
        return;
    }
    this->gearValue = received_value;
}

void gearObject::on_joy_gear_set(const std::shared_ptr<vsomeip::message> &_request) {
    std::cout << "@@@@     JOY GEAR SET FUNCTION" << std::endl;
    std::shared_ptr<vsomeip::payload> payload = _request->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int*>(payload->get_data());
        std::cout << "SERVER: Received int: " << received_value << std::endl;
    } else {
        std::cerr << "SERVER: Invalid payload size!" << std::endl;
        return;
    }
    this->gearValue = received_value;
    
    //response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&received_value),
      reinterpret_cast<vsomeip::byte_t *>(&received_value) + sizeof(int));    
    
    payload->set_data(its_payload_data);
    its_response->set_payload(payload);
    // its_response->set_method(JOY_GEAR_RESPONSE_MID);
    app_->send(its_response);

}


//TODO: 2025-02-16 add subscribe to IC and HU. and add mutex in requesting part not to be on deadlock.
