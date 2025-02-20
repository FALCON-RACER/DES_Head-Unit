#include "battery.hpp"

batteryObject::batteryObject(uint32_t _cycle) :
    app_(vsomeip::runtime::get()->create_application("battery")),
    is_registered_(false),
    cycle_(_cycle),
    blocked_(false),
    running_(true),
    is_offered_(false),
    offer_thread_(std::bind(&batteryObject::run, this)),
    notify_thread_(std::bind(&batteryObject::notify, this)),
    speedData(0),
    voltage(0),
    file(-1){
    battery_thread_ = std::thread(&batteryObject::getBatteryVoltage, this);
    if (!initI2C()) {
        std::cout << "Failed to initialize I2C interface.";
    }

}

bool batteryObject::init() {
    std::lock_guard<std::mutex> its_lock(mutex_);

    if (!app_->init()) {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }
    //서비스에 따라서 service id 변경.

    app_->register_state_handler(
            std::bind(&batteryObject::on_state, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            BATTERY_INSTANCE_ID,
            GET_METHOD_ID,
            std::bind(&batteryObject::on_get, this, std::placeholders::_1));

    app_->register_message_handler(
            VEHICLE_SERVICE_ID,
            BATTERY_INSTANCE_ID,
            SET_METHOD_ID,
            std::bind(&batteryObject::on_set, this, std::placeholders::_1));

    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(VEHICLE_EVENTGROUP_ID);

    // event provide function
    app_->offer_event(
            VEHICLE_SERVICE_ID,
            BATTERY_INSTANCE_ID,
            BATTERY_EVENT_ID,
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

void batteryObject::start() {
    app_->start();
}

void batteryObject::stop() {
    std::cout << "SERVER : stop" << std::endl;
    running_ = false;
    blocked_ = true;
    condition_.notify_one();
    notify_condition_.notify_one();
    app_->clear_all_handler();
    if (file >= 0) {
        close(file); // Close the I2C device file
        file = -1;   // Reset file descriptor to indicate it's closed
    }

    stop_offer();
    
    if (offer_thread_.joinable()) offer_thread_.join();
    if (notify_thread_.joinable()) notify_thread_.join();
    if (battery_thread_.joinable()) battery_thread_.join();

    app_->stop();

}

void batteryObject::offer() {
    std::cout << "SERVER : offer" << std::endl;
    std::lock_guard<std::mutex> its_lock(notify_mutex_);
    app_->offer_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    is_offered_ = true;
    notify_condition_.notify_one();
    battery_condition_.notify_one();
    std::cout << "battery condition notified" << std::endl;

}

void batteryObject::stop_offer() {
    std::cout << "SERVER : stop offer" << std::endl;
    app_->stop_offer_service(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID);
    is_offered_ = false;
}

void batteryObject::run() {
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

//TODO : change it into uint32_t type
void batteryObject::notify() {
    while (running_) {
        std::unique_lock<std::mutex> its_lock(notify_mutex_);
        while (!is_offered_ && running_) 
            notify_condition_.wait(its_lock);
        while(is_offered_ && running_) {
                    // {
                    //     std::vector<vsomeip::byte_t> speed_data(
                    //     reinterpret_cast<vsomeip::byte_t *>(&speedData),
                    //     reinterpret_cast<vsomeip::byte_t *>(&speedData) + sizeof(speedData));
                    //     {
                    //         std::lock_guard<std::mutex> its_lock(payload_mutex_);
                    //         payload_->set_data(speed_data);
                    //         app_->notify(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID, payload_);
                    //     }
                    // }
                    {
                        std::vector<vsomeip::uint8_t> voltage_data(
                        reinterpret_cast<vsomeip::uint8_t *>(&voltage),
                        reinterpret_cast<vsomeip::uint8_t *>(&voltage) + sizeof(voltage));
                        {
                            std::lock_guard<std::mutex> its_lock(payload_mutex_);
                            payload_->set_data(voltage_data);
                            app_->notify(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID, payload_);
                        }
                    }
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
    }
}

void batteryObject::on_state(vsomeip::state_type_e _state) {
    std::cout << "Application " << app_->get_name() << " is "
              << (_state == vsomeip::state_type_e::ST_REGISTERED ?
                  "registered." : "deregistered.") << std::endl;
    is_registered_ = (_state == vsomeip::state_type_e::ST_REGISTERED);
    
}

void batteryObject::on_get(const std::shared_ptr<vsomeip::message> &_message) {
    std::shared_ptr<vsomeip::message> its_response =
        vsomeip::runtime::get()->create_response(_message);
    {
        std::lock_guard<std::mutex> its_lock(payload_mutex_);
        its_response->set_payload(payload_);
    }
    app_->send(its_response);
    
}

void batteryObject::on_set(const std::shared_ptr<vsomeip::message> &_message) {
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
    app_->notify(VEHICLE_SERVICE_ID, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID, payload_);

}

//TODO: change it into battery service code.
/*
void batteryObject::getBatteryVoltage() {
    float filtered_speed = 0.0f;
    float weight = 0.6;

    std::cout << "running : " << running_ << std::endl;
    while (running_) {
        std::unique_lock<std::mutex> its_lock(can_mutex_);
        while (!is_offered_ && running_) 
            battery_condition_.wait(its_lock);
        while (is_offered_ && running_) 
            {
                {
                    filtered_speed += 0.2;
                    this->speedData = filtered_speed;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(7));

            }
    }

}
*/

// Destructor: Cleans up the resources (closes the I2C file descriptor)

bool batteryObject::initI2C() {
    // Open the I2C bus
    if ((file = open(device, O_RDWR)) < 0) {
        std::cout << "Failed to open the I2C bus";
        return false;
    }

    // Set the I2C address for the slave device
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        std::cout << "Failed to acquire bus access and/or talk to slave.";
        close(file);
        return false;
    }

    return true;
}

uint16_t batteryObject::readRegister() {
    char buf[2];
    buf[0] = reg;

    if (file < 0) {
        std::cout << "I2C file is not initialized.";
        return -1;
    }

    // Write the register address to the I2C bus
    if (write(file, buf, 1) != 1) {
        std::cout << "Failed to write to the I2C bus.";
        std::cout << "Error still exists";
        return -1;
    }
    usleep(1000); // Time delay to read back from I2C

    // Read the data from the I2C bus
    if (read(file, buf, 2) != 2) {
        std::cout << "Failed to read from the I2C bus.";
        return -1;
    }

    uint16_t readValue = (buf[0] << 8) + buf[1];
    return readValue;
}

uint8_t batteryObject::getBatteryVoltage() {
    // The battery voltage is stored in register 0x02
    // uint16_t voltageRaw = readRegister();
    int voltage = 11;
    // uint8_t voltage = ((voltageRaw>>3)*4.0)/1000;
    std::cout << "Battery Voltage: " << voltage << std::endl;
    return voltage;
}