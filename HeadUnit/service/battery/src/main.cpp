#include "battery.h"
#include "vehiclebattery.h"
#include <cstdint>
#include <unistd.h>
#include <iostream>

int main() {
    Battery battery;
    VehicleBattery vehiclebattery;

    uint8_t battery_percent = 0;

    while(1) {
        std::cout << "Calling getBatteryVoltage..." << std::endl;
        uint8_t voltage = vehiclebattery.getBatteryVoltage();
        std::cout << "getBatteryVoltage called." << std::endl;
        std::cout<<"voltage : " << voltage << std::endl;
        int lowVoltage = 9;
        float diffVoltage = 3.45;
        if (voltage == -1) {
            std::cout << "Failed to read battery voltage." << std::endl << std::flush;
            return -1;
        }
        float battery_percent = (((voltage - lowVoltage) / diffVoltage )*100);
        std::cout << "Percentage: "<< battery_percent << "%" << std::endl << std::flush;

        if ((battery_percent-100)>0) {battery_percent = 100;}
        else if ((battery_percent)<0) {battery_percent = 0;}

        battery.setBatteryVoltage(battery_percent);
	usleep(500000);
    }
}
