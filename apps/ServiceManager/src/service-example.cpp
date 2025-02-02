// service_sample.cpp
#include "service-example.hpp"

service_sample::service_sample(uint32_t _cycle, int _service) :
        app_(vsomeip::runtime::get()->create_application()),
        is_registered_(false),
        cycle_(_cycle),
        service_(_service),
        blocked_(false),
        running_(true),
        is_offered_(false),
        offer_thread_(std::bind(&service_sample::run, this)),
        notify_thread_(std::bind(&service_sample::notify, this)),
        speedData(0){
        if (_service == 1) { // 서비스 1일 경우
            CAN_thread_ = std::thread(&service_sample::canDataReceive, this);
            std::cout << "CAN THREAD CREATED" << std::endl;
        }
        if (_service == 2) { // 서비스 2일 경우
            battery_thread_ = std::thread(&service_sample::getBatteryVoltage, this);
            std::cout << "BATTERY THREAD CREATED" << std::endl;
        }
    
}

int service_sample::getService() {
    return service_;
}

bool service_sample::init() {
    std::lock_guard<std::mutex> its_lock(mutex_);

    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }

    app_->register_state_handler(
            std::bind(&service_sample::on_state, this, std::placeholders::_1));

    app_->register_message_handler(
            SAMPLE_SERVICE_ID,
            SAMPLE_INSTANCE_ID,
            SAMPLE_GET_METHOD_ID,
            std::bind(&service_sample::on_get, this, std::placeholders::_1));

    app_->register_message_handler(
            SAMPLE_SERVICE_ID,
            SAMPLE_INSTANCE_ID,
            SAMPLE_SET_METHOD_ID,
            std::bind(&service_sample::on_set, this, std::placeholders::_1));

    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(SAMPLE_EVENTGROUP_ID);

    // event provide function
    app_->offer_event(
            SAMPLE_SERVICE_ID,
            SAMPLE_INSTANCE_ID,
            SAMPLE_EVENT_ID,
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

void service_sample::start() {
    app_->start();
}

void service_sample::stop() {
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
    if (battery_thread_.joinable()) battery_thread_.join();

    app_->stop();
}

void service_sample::offer() {
    std::cout << "SERVER : offer" << std::endl;
    std::lock_guard<std::mutex> its_lock(notify_mutex_);
    app_->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    is_offered_ = true;
    notify_condition_.notify_one();
    if (service_ == SPEED) {
        CAN_condition_.notify_one();
        std::cout << "can condition notified" << std::endl;
    }
    else if (service_ == BATTERY) {
        battery_condition_.notify_one();
        std::cout << "battery condition notified" << std::endl;
    }
}

void service_sample::stop_offer() {
    std::cout << "SERVER : stop offer" << std::endl;
    app_->stop_offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    is_offered_ = false;
}

void service_sample::on_state(vsomeip::state_type_e _state) {
    std::cout << "Application " << app_->get_name() << " is "
              << (_state == vsomeip::state_type_e::ST_REGISTERED ?
                  "registered." : "deregistered.") << std::endl;
    is_registered_ = (_state == vsomeip::state_type_e::ST_REGISTERED);
}

void service_sample::on_get(const std::shared_ptr<vsomeip::message> &_message) {
    std::shared_ptr<vsomeip::message> its_response =
        vsomeip::runtime::get()->create_response(_message);
    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        its_response->set_payload(payload_);
    }
    app_->send(its_response);
}

void service_sample::on_set(const std::shared_ptr<vsomeip::message> &_message) {
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
    app_->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload_);
}

void service_sample::run() {
    std::unique_lock<std::mutex> its_lock(mutex_);
    while (!blocked_) condition_.wait(its_lock);

    bool is_offer = true;
    while (running_) {
        if (is_offer) offer();
        else stop_offer();

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        for (int i = 0; i < 10 && running_; i++)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // is_offer = !is_offer;
    }
}

void service_sample::notify() {
    while (running_) {
        std::unique_lock<std::mutex> its_lock(notify_mutex_);
        while (!is_offered_ && running_) 
            notify_condition_.wait(its_lock);
        while(is_offered_ && running_) {

            if (service_ == SPEED) 
            {
                // std::cout << "In notify function. speedData = " << speedData << std::endl;
                    {
                        std::vector<vsomeip::byte_t> speed_data(
                        reinterpret_cast<vsomeip::byte_t *>(&speedData),
                        reinterpret_cast<vsomeip::byte_t *>(&speedData) + sizeof(speedData));
                        {
                            std::lock_guard<std::mutex> its_lock(payload_mutex_);
                            payload_->set_data(speed_data);
                            app_->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload_);
                        }
                    }
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
            }
            // else if (service_ == BATTERY) 
            // {
            //     std::cout << "In notify function. Battery = " << speedData << std::endl;
            //         {
            //             std::vector<vsomeip::byte_t> speed_data(
            //             reinterpret_cast<vsomeip::byte_t *>(&speedData),
            //             reinterpret_cast<vsomeip::byte_t *>(&speedData) + sizeof(speedData));
            //             {
            //                 std::lock_guard<std::mutex> its_lock(payload_mutex_);
            //                 payload_->set_data(speed_data);
            //                 app_->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload_);
            //             }
            //         }
            //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // }
            // else if (service_ == BATTERY) {
            // TODO: create payload for uint_8 type  
            // }
        }
    }
}

void service_sample::canDataReceive() {
    // -> 2025.01.07.20:41 기준 mutex 관련 문제 발생. 아래 2줄로 인해 이 스레드 실행이 안됨
    /* std::unique_lock<std::mutex> its_lock(mutex_); // 뮤텍스 잠금
     while (!blocked_) condition_.wait(its_lock);  // init() 완료 대기 */

    float filtered_speed = 0.0f;
    float weight = 0.6;

    CANReceiver canreceiver = CANReceiver("can0");

    while (running_) {
        std::unique_lock<std::mutex> its_lock(can_mutex_);
        while (!is_offered_ && running_) 
            CAN_condition_.wait(its_lock);
        while (is_offered_ && running_ && canreceiver.canRead()) 
            {
                {
                    filtered_speed = (1 - weight) * filtered_speed + weight * canreceiver.getSpeed();
                    this->speedData = filtered_speed;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(7));

            }
    }

    // while (running_) {
    //     std::unique_lock<std::mutex> its_lock(can_mutex_);
    //     while (!is_offered_ && running_) 
    //         CAN_condition_.wait(its_lock);
    //     while (is_offered_ && running_) 
    //         {
    //             {
    //                 filtered_speed += 0.1;
    //                 this->speedData = filtered_speed;
    //             }
    //             std::this_thread::sleep_for(std::chrono::milliseconds(7));

    //         }
    // }
}

void service_sample::getBatteryVoltage() {

    float filtered_speed = 0.0f;


    while (running_) {
        std::unique_lock<std::mutex> its_lock(battery_mutex_);
        while (!is_offered_ && running_) 
            battery_condition_.wait(its_lock);
        while (is_offered_ && running_) 
            {
                // std::cout << "BATTERY in" << std::endl;
                // speedData += 0.1;
                {filtered_speed += 0.1;
                this->speedData = filtered_speed;}
                std::this_thread::sleep_for(std::chrono::milliseconds(7));
                // {
                //     filtered_speed += 0.1;
                //     this->speedData = filtered_speed;
                // }
                // std::this_thread::sleep_for(std::chrono::milliseconds(7));

            }
    }

}   