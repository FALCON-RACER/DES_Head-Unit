#pragma once
#ifndef __ADAFRUIT_PCA9685_HPP__
# define __ADAFRUIT_PCA9685_HPP__

#include <iostream>
#include <unistd.h>
#include <pigpio.h>
#include <cmath>

// Registers/etc:
#define PCA9685_ADDRESS		0x40
#define MODE1				0x00
#define MODE2				0x01
#define PRESCALE			0xFE
#define LED0_ON_L			0x06
#define LED0_ON_H			0x07
#define LED0_OFF_L			0x08
#define LED0_OFF_H			0x09
#define ALL_LED_ON_L		0xFA
#define ALL_LED_ON_H		0xFB
#define ALL_LED_OFF_L		0xFC
#define ALL_LED_OFF_H		0xFD

// Bits:
#define RESTART				0x80
#define SLEEP				0x10
#define ALLCALL				0x01
#define INVRT				0x10
#define OUTDRV				0x04

class PCA9685 
{
public:
	PCA9685(int i2c_bus, int address = PCA9685_ADDRESS);
	~PCA9685();

	void setPWMFreq(float freqHz);
	void setPWM(int channel, int on, int off);
	void setAllPWM(int on, int off);

private:
	int	fd;
	int	i2c_bus;
	int	addr;
};

#endif