#include "vehiclebattery.h"
#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

VehicleBattery::VehicleBattery() : file(-1) {
    if (!initI2C()) {
        std::cout << "Failed to initialize I2C interface.";
    }
}

// Destructor: Cleans up the resources (closes the I2C file descriptor)
VehicleBattery::~VehicleBattery() {
    if (file >= 0) {
        close(file); // Close the I2C device file
        file = -1;   // Reset file descriptor to indicate it's closed
    }
}

bool VehicleBattery::initI2C() {
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

uint16_t VehicleBattery::readRegister() {
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

/**
 * @Battery Percentage calculation details
 *
 * Full charge voltage is 12.45V
 * RPi turns off when the bus voltage is 8.4V
 * Hence the low voltage is set to 9V
 *
 *
 *
 * The difference in voltage is 12.4 - 9 = 3.45
 * battery_percentage = ((actual voltage - low voltage) / diffVoltage) * 100
 */

uint8_t VehicleBattery::getBatteryVoltage() {
    // The battery voltage is stored in register 0x02
    uint16_t voltageRaw = readRegister();
    uint8_t voltage = ((voltageRaw>>3)*4.0)/1000;
    std::cout << "Battery Voltage: " << voltage;
    return voltage;
}