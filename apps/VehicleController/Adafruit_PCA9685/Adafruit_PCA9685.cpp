#include "Adafruit_PCA9685.hpp"

PCA9685::PCA9685(int i2c_bus, int addr) : i2c_bus(i2c_bus), addr(addr)
{
	fd = i2cOpen(i2c_bus, addr, 0);
	if (fd < 0)
	{
		std::cerr << "Failed to open I2C bus" << std::endl;
		exit(1);
	}
	setAllPWM(0, 0);
	i2cWriteByteData(fd, MODE2, OUTDRV);
	i2cWriteByteData(fd, MODE1, ALLCALL);
	usleep(5000); // wait for oscillator
	int mode1 = i2cReadByteData(fd, MODE1);
	mode1 = mode1 & ~SLEEP; // wake up (reset sleep)
	i2cWriteByteData(fd, MODE1, mode1);
	usleep(5000); // wait for oscillator
}

PCA9685::~PCA9685()
{
	if (gpioInitialise() >= 0)
		i2cClose(fd);
}

void PCA9685::setPWMFreq(float freqHz)
{
	float prescaleval = 25000000.0;
	prescaleval /= 4096.0;
	prescaleval /= freqHz;
	prescaleval -= 1.0;
	int prescale = std::floor(prescaleval + 0.5);
	int oldmode = i2cReadByteData(fd, MODE1);
	int newmode = (oldmode & 0x7F) | 0x10; // sleep
	i2cWriteByteData(fd, MODE1, newmode);  // go to sleep
	i2cWriteByteData(fd, PRESCALE, prescale);
	i2cWriteByteData(fd, MODE1, oldmode);
	usleep(5000);
	i2cWriteByteData(fd, MODE1, oldmode | 0x80);
}

void PCA9685::setPWM(int channel, int on, int off)
{
	i2cWriteByteData(fd, LED0_ON_L + 4 * channel, on & 0xFF);
	i2cWriteByteData(fd, LED0_ON_H + 4 * channel, on >> 8);
	i2cWriteByteData(fd, LED0_OFF_L + 4 * channel, off & 0xFF);
	i2cWriteByteData(fd, LED0_OFF_H + 4 * channel, off >> 8);
}

void PCA9685::setAllPWM(int on, int off)
{
	i2cWriteByteData(fd, ALL_LED_ON_L, on & 0xFF);
	i2cWriteByteData(fd, ALL_LED_ON_H, on >> 8);
	i2cWriteByteData(fd, ALL_LED_OFF_L, off & 0xFF);
	i2cWriteByteData(fd, ALL_LED_OFF_H, off >> 8);
}
