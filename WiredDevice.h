#ifndef WIREDDEVICE_H
#define WIREDDEVICE_H

#include <Wire.h>

class WiredDevice
{
  protected:
	uint8_t addr;
	uint8_t readRegister(uint8_t reg, uint8_t *data = NULL)
	{
		Wire.beginTransmission(addr);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom((uint8_t)addr, (uint8_t)1);
		if (!Wire.available())
			return FAILED;
		if (data != NULL)
		{
			*data = Wire.read();
		}
		return SUCCESS;
	};

	uint8_t readRegisterDebug(uint8_t reg)
	{
		Wire.beginTransmission(addr);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom((uint8_t)addr, (uint8_t)1);
		if (!Wire.available())
			return FAILED;
		return Wire.read();
	};

	uint8_t writeRegister(uint8_t reg, uint8_t value)
	{
		Wire.beginTransmission(addr);
		Wire.write(reg);
		Wire.write(value);
		Wire.endTransmission();
		return SUCCESS;
	}
};

#endif
