#include "battery.h"
#include "vehiclebattery.h"
#include <cstdint>
#include <iostream>

int main() {
    Battery battery;
    VehicleBattery vehiclebattery;

    uint8_t battery_percent = 0;

    while(1) {
        uint8_t voltage = vehiclebattery.getBatteryVoltage();
        std::cout<<"voltage : "<<vehiclebattery.getBatteryVoltage()<<std::endl;
        uint8_t lowVoltage = 9;
        uint8_t diffVoltage = 3.45;
        if (voltage == -1) {
            std::cout << "Failed to read battery voltage." << std::endl;
            return -1;
        }
        uint8_t battery_percent = (((voltage - lowVoltage) / diffVoltage )*100);
        std::cout << battery_percent << "%";

        if ((battery_percent-100)>0) {battery_percent = 100;}
        else if ((battery_percent)<0) {battery_percent = 0;}

        battery.setBatteryVoltage(battery_percent);
    }
}