#include <../src-gen/core/proxy/battery/v1/commonapi/BatteryStatusProxy.hpp>
#include <CommonAPI/CommonAPI.hpp>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

using namespace v1::commonapi;

// void onBatteryStatusChanged(const BatteryStatusProxyBase::BatteryAttribute::ValueType& batteryStatus) {
//     std::cout << "Battery Status Updated:" << std::endl;
//     std::cout << " - Level: " << batteryStatus << "%" << std::endl;
// }

int main() {
    // Initialize CommonAPI runtime
    auto runtime = CommonAPI::Runtime::get();

    // Instantiate the BatteryStatusProxy
    // std::shared_ptr<BatteryStatusProxy<>> batteryStatusProxy = 
    //     runtime->buildProxy<CommonAPI::Proxy, BatteryStatusProxy>("local", "BatteryStatus");

    std::shared_ptr<BatteryStatusProxy<>> batteryProxy = runtime->buildProxy<BatteryStatusProxy>("local", "BatteryStatus", "InstrumentCluster_Battery_Proxy");

    // Check if the proxy is available
    if (!batteryProxy) {
        std::cerr << "Error: Failed to initialize the battery status proxy." << std::endl;
        return 1;
    }

    // Wait for the proxy to be available
    while (!batteryProxy->isAvailable()) {
        std::cout << "Waiting for battery status service..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Connected to battery status service!" << std::endl;

    batteryProxy->getBatteryAttribute().getChangedEvent().subscribe([&](const float& battery){
        std::cout << "Battery Status Updated:" << std::endl;
        std::cout << " - Level: " << battery << "%" << std::endl;
    });

    // Subscribe to the Battery attribute
    // batteryProxy->getBatteryAttribute().subscribe(
    //     onBatteryStatusChanged,
    //     [](const CommonAPI::CallStatus& callStatus) {
    //         if (callStatus == CommonAPI::CallStatus::SUCCESS) {
    //             std::cout << "Successfully subscribed to battery status updates." << std::endl;
    //         } else {
    //             std::cerr << "Failed to subscribe to battery status updates." << std::endl;
    //         }
    //     });

    // Keep the main thread alive to receive updates
    std::cout << "Listening for battery status updates..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}
