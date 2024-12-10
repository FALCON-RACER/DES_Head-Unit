#include <cstdint>
#include <exception>

class INA219Reader
{
public:
	INA219Reader();
	~INA219Reader();

	float	readBusVoltage() const;
    float	readShuntVoltage() const;\
	float	amper;
	float	readCurrent() const;
    float   readAmper() const;
private:
	const char*	I2C_BUS = "/dev/i2c-1";

    const int8_t	INA219_ADDRESS = 0x41;
    const int8_t	SHUNT_VOLTAGE_REGISTER = 0x01;
	const int8_t	BUS_VOLTAGE_REGISTER = 0x02;
    const int8_t	AMPERAGE_REGISTER = 0x04;
    float BUS_VOLTAGE_MULTIPLIER = 0.004f;
    float SHUNT_VOLTAGE_MULTIPLIER = 0.01f;
    float AMPER_MULTIPLAYER = 0.001f;

	int	fd;

	void	openI2cBus();
	void	setI2cDeviceAddress() const;
	void	writeRegisterValueToI2cDevice(unsigned char reg) const;
	int16_t readRegisterValueFromI2cDevice() const;

	class I2cBusOpenFailException : public std::exception
	{
		public:
			const char*	what() const throw();
	};
	class SetI2cDeviceAddressFailException : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class WriteRegisterValueToI2cDeviceFailException : public std::exception
	{
		public:
			const char* what() const throw();
	};
	class ReadRegisterValueFromI2cDeviceFailException : public std::exception
	{
		public:
			const char* what() const throw();
	};
};
