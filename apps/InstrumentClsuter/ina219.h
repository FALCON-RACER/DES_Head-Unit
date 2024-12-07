#ifndef INA219_H
#define INA219_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

extern "C" {
#include <i2c/smbus.h>
}

#define CALIBRATION_VALUE 4096
#define MODE_SHUNT_AND_BUS_CONTINUOUS   0x0007
#define SHUNT_ADC_12BIT_128SAMPLES      0x0078
#define BUS_ADC_12BIT_128SAMPLES        0x0780
#define BUS_ADC_12BIT                   0x0180
#define PGA_GAIN_8                      0x1800
#define BUS_VOLTAGE_RANGE_32V           0x2000

class INA219
{
private:
    int _file;
    int _address;

    uint16_t readRegister(uint8_t registerNumber);
    void writeRegister(uint8_t registerNumber, uint16_t value);
    void setCalibration();
    void setConfiguration();
    float getBusVoltage();
    float getShuntVoltage();

public:
    INA219();
    INA219(const char *filename, int address);
    float getBatteryVoltage();
};

#endif
