#include "PiRacer/PiRacer.hpp"
#include "Gamepad/ShanwanGamepad.hpp"
#include "../ServiceManager/src/server.hpp"

/**
 * A : DRIVE
 * B : NEUTRAL
 * X : REVERSE
 * Y : PARKING
 */

enum Gear {
    PARKING,
    REVERSE,
    NETURAL,
    DRIVE
};

Gear gear = PARKING;
std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;

// sending the actual data (will be gear data) to server.
void send_gear_data(int data) {
    std::unique_lock<std::mutex> its_lock(mutex);
    condition.wait(its_lock);

    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(GEAR_INSTANCE_ID);
    request->set_method(JOY_GEAR_SET_MID);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
        reinterpret_cast<vsomeip::byte_t *>(&data),
        reinterpret_cast<vsomeip::byte_t *>(&data) + sizeof(int));
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);
    std::cout << "CLIENT : DATA SENDED" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// When response come, print the payload from server.
void on_message(const std::shared_ptr<vsomeip::message> &_response) {
    std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int *>(payload->get_data());
        std::cout << "SERVER: Received int: " << received_value << std::endl;
    }
    else {
        std::cerr << "SERVER: Invalid payload size!" << std::endl;
        return;
    }
}

void initVSOMEIP() {
    app = vsomeip::runtime::get()->create_application("gear");
    app->init();
    app->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, on_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, JOY_GEAR_SET_MID, on_message);
    std::thread sender(run);
    app->start();
}

int main() {
    atexit(gpioTerminate);

    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    initVSOMEIP();
    PiRacer racer;
    ShanWanGamepad gamepad;

    float steering = 0;
    float throttle = 0;

    while (true) {
        ShanWanGamepadInput input = gamepad.read_data();

        if (input.button) {
            if (input.button_a)
                gear = DRIVE;
            else if (input.button_b)
                gear = NEUTRAL;
            else if (input.button_x)
                gear = REVERSE;
            else if (input.button_y)
                gear = PARKING;

            if (gear == PARKING || gear == NEUTRAL) {
                racer.setSteeringPercent(0);
                racer.setThrottlePercent(0);
            }
            send_gear_data(gear);
        }
        else {
            if (gear == PARKING || gear == NEUTRAL)
                continue;

            steering = 0;
            throttle = 0;

            steering = input.analog_stick_left.x;
            if (input.analog_stick_right.x <= 0)
                throttle = input.analog_stick_right.x * -0.7;
            if (gear == DRIVE)
                throttle *= -1;

            racer.setSteeringPercent(steering);
            racer.setThrottlePercent(throttle);
        }
    }
}
