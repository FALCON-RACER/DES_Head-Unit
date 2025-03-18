#include "speed.hpp"

speedObject::speedObject(uint32_t _cycle) :
    app_(vsomeip::runtime::get()->create_application("speed")),
    is_registered_(false),
    cycle_(_cycle),
    blocked_(false),
    running_(true),
    is_offered_(false),
    offer_thread_(std::bind(&speedObject::run, this)),
    notify_thread_(std::bind(&speedObject::notify, this)),
    speedData(0) {
      CAN_thread_ = std::thread(&speedObject::canDataReceive, this);
    }

bool speedObject::init() {
    std::lock_guard<std::mutex> its_lock(mutex_);

    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }
    
    app_->register_state_handler(
            std::bind(&speedObject::on_state, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            SPEED_INSTANCE_ID,
            GET_METHOD_ID,
            std::bind(&speedObject::on_get, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            SPEED_INSTANCE_ID,
            SET_METHOD_ID,
            std::bind(&speedObject::on_set, this, std::placeholders::_1));

    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(VEHICLE_EVENTGROUP_ID);

    // event provide function
    app_->offer_event(
            VEHICLE_SERVICE_ID,
            SPEED_INSTANCE_ID,
            SPEED_EVENT_ID,
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

void speedObject::start() {
    app_->start();
}

void speedObject::stop() {
    std::cout << "SERVER : stop" << std::endl;
    running_ = false;
    blocked_ = true;
    condition_.notify_one();
    notify_condition_.notify_one();
    app_->clear_all_handler();
    stop_offer();
    
    if (offer_thread_.joinable()) offer_thread_.join();
    if (notify_thread_.joinable()) notify_thread_.join();
    if (CAN_thread_.joinable()) CAN_thread_.join();

    app_->stop();

}

void speedObject::offer() {
    std::cout << "SERVER : offer" << std::endl;
    std::lock_guard<std::mutex> its_lock(notify_mutex_);
    app_->offer_service(VEHICLE_SERVICE_ID, SPEED_INSTANCE_ID);
    is_offered_ = true;
    notify_condition_.notify_one();
    CAN_condition_.notify_one();
    std::cout << "can condition notified" << std::endl;

}

void speedObject::stop_offer() {
    std::cout << "SERVER : stop offer" << std::endl;
    app_->stop_offer_service(VEHICLE_SERVICE_ID, SPEED_INSTANCE_ID);
    is_offered_ = false;
}

void speedObject::run() {
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

void speedObject::notify() {
    while (running_) {
        std::unique_lock<std::mutex> its_lock(notify_mutex_);
        while (!is_offered_ && running_) 
            notify_condition_.wait(its_lock);
        while(is_offered_ && running_) {
                    {   
                        std::vector<vsomeip::byte_t> speed_data(
                        reinterpret_cast<vsomeip::byte_t *>(&speedData),
                        reinterpret_cast<vsomeip::byte_t *>(&speedData) + sizeof(speedData));
                        {
                            std::lock_guard<std::mutex> its_lock(payload_mutex_);
                            payload_->set_data(speed_data);
                            app_->notify(VEHICLE_SERVICE_ID, SPEED_INSTANCE_ID, SPEED_EVENT_ID, payload_);
                        }
                    }
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
    }
}

void speedObject::on_state(vsomeip::state_type_e _state) {
    std::cout << "Application " << app_->get_name() << " is "
              << (_state == vsomeip::state_type_e::ST_REGISTERED ?
                  "registered." : "deregistered.") << std::endl;
    is_registered_ = (_state == vsomeip::state_type_e::ST_REGISTERED);
    
}

void speedObject::on_get(const std::shared_ptr<vsomeip::message> &_message) {
    std::shared_ptr<vsomeip::message> its_response =
        vsomeip::runtime::get()->create_response(_message);
    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        its_response->set_payload(payload_);
    }
    app_->send(its_response);
    
}

void speedObject::on_set(const std::shared_ptr<vsomeip::message> &_message) {
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
    app_->notify(VEHICLE_SERVICE_ID, SPEED_INSTANCE_ID, SPEED_EVENT_ID, payload_);

}

void speedObject::canDataReceive() {
    std::cout << "can Data Receive started" << std::endl;
    float filtered_speed = 0.0f;
    float weight = 0.6;
    CANReceiver receiver;
    // CANReceiver receiver("can0");

    while (running_) {
        std::unique_lock<std::mutex> its_lock(can_mutex_);
        while (!is_offered_ && running_) 
            CAN_condition_.wait(its_lock);
        while (is_offered_ && running_) 
            {
                {   
                    filtered_speed = receiver.get();
                    // receiver.canRead();
                    // filtered_speed = receiver.getSpeed();
                    // filtered_speed = (1-weight)*filtered_speed + (weight)*canData.getSpeed();
                    this->speedData = filtered_speed;
                    std::cout << "[Speed server] in can data receive loop : filtered_speed : " << filtered_speed << std::endl;
                    // if (filtered_speed >= 100.0f)
                    //     filtered_speed = 0.0f;
                    // filtered_speed += 0.1;
                    // this->speedData = filtered_speed;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));

            }
    }
    
    // while (running_) {
    //     std::unique_lock<std::mutex> its_lock(can_mutex_);
    //     while (!is_offered_ && running_) 
    //         CAN_condition_.wait(its_lock);
    //     while (is_offered_ && running_) 
    //         {
    //             {   
    //                 if (filtered_speed >= 100.0f)
    //                     filtered_speed = 0.0f;
    //                 filtered_speed += 0.1;
    //                 this->speedData = filtered_speed;
    //             }
    //             std::this_thread::sleep_for(std::chrono::milliseconds(300));

    //         }
    // }
}
