// service_sample.cpp
#include "service-example.hpp"

service_sample::service_sample(uint32_t _cycle) :
        app_(vsomeip::runtime::get()->create_application()),
        is_registered_(false),
        cycle_(_cycle),
        blocked_(false),
        running_(true),
        is_offered_(false),
        offer_thread_(std::bind(&service_sample::run, this)),
        notify_thread_(std::bind(&service_sample::notify, this)) {
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

void service_sample::offer() {
    std::lock_guard<std::mutex> its_lock(notify_mutex_);
    app_->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    is_offered_ = true;
    notify_condition_.notify_one();
}

void service_sample::stop_offer() {
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // is_offer = !is_offer;
    }
}

void service_sample::notify() {
    vsomeip::byte_t its_data[10];
    uint32_t its_size = 1;

    while (running_) {
        std::unique_lock<std::mutex> its_lock(notify_mutex_);
        while (!is_offered_ && running_) notify_condition_.wait(its_lock);
        // service에 따라 다르게 작동하게. 
        // speed part 

        // battery part
        while (is_offered_ && running_) {
            if (its_size == sizeof(its_data)) its_size = 1;

            for (uint32_t i = 0; i < its_size; ++i)
                its_data[i] = static_cast<uint8_t>(i);

            {
                std::lock_guard<std::mutex> its_lock(payload_mutex_);
                payload_->set_data(its_data, its_size);
                app_->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload_);
            }
            its_size++;
            std::this_thread::sleep_for(std::chrono::milliseconds(cycle_));
        }

    }
}
