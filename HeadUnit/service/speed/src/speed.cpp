#include "speed.h"

Speed::Speed() {
    runtime = CommonAPI::Runtime::get();

    speedService = std::make_shared<SpeedStubImpl>();
    speedServiceInit();
}

Speed::~Speed() {
}

void Speed::speedServiceInit() {
    while (!runtime->registerService("local", "speed", speedService, "Speed_Service")) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Successfully Registered Speed Service!" << std::endl;

}

void Speed::setSpeed(float speed) {
    speedService->setSpeedAttribute(speed);
}
