#ifndef VEHICLE_BATTERY_H
#define VEHICLE_BATTERY_H
#include <cstdint>

class VehicleBattery {
public:
    VehicleBattery();
    ~VehicleBattery();

    uint8_t getBatteryVoltage();

private:
    int file;
    const char *device = "/dev/i2c-1"; // I2C bus device
    int addr = 0x41; // The I2C address of the battery monitoring device
    char reg = 0x02;
    bool initI2C();  // Method to initialize the I2C interface
    uint16_t readRegister(); // Method to read a value from a register
};

#endif // VEHICLE_BATTERY_H