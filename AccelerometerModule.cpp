#include "AccelerometerModule.h"
#include "Debug.h"
#include "Notification.h"

#define putDeviceInStandByMode writeRegister(AM_REG_CTRL_REG1, readRegister(AM_REG_CTRL_REG1) & 0xFE)
#define putDeviceInActiveMode writeRegister(AM_REG_CTRL_REG1, readRegister(AM_REG_CTRL_REG1) | 1)

AccelerometerModule::AccelerometerModule()
{
	deboundCounterNotification = 5000; // Should not notify again within 5s
	thresholdForMotion = 25;
	thresholdForTap = 12;
	hasInterrupt = false;
	lastAlertTime = 0;
}

uint8_t AccelerometerModule::setup(uint8_t addr)
{
	Wire.begin();
	this->addr = addr;
	/* Check connection */
	uint8_t deviceId;
	readRegister(AM_REG_WHOAMI, &deviceId);
	if (deviceId != 0x1A)
	{
		DEBUG.println("Acclerometer Sensor is not detected!");
		return FAILED;
	}
	writeRegister(AM_REG_CTRL_REG2, 0x40); // Reset software stack
	uint8_t flag;
	do
	{
		readRegister(AM_REG_CTRL_REG2, &flag);
	} while (flag & 0x40);
	// Put device in standby mode to modify registers' values
	putDeviceInStandByMode;
	// Enable Auto Sleep/Wake
	uint8_t regValue;
	readRegister(AM_REG_CTRL_REG2, &regValue);
	writeRegister(AM_REG_CTRL_REG2, 0x04 | regValue);
	// Set sample rate for wake and sleep mode. Wake sample rate is 100Hz. Sleep sample rate is 6.25Hz.
	readRegister(AM_REG_CTRL_REG1, &regValue);
	writeRegister(AM_REG_CTRL_REG1, (regValue & 0x5E) | 0x58);
	// Wake - High resolution for oversampling. Sleep - Low power.
	readRegister(AM_REG_CTRL_REG2, &regValue);
	writeRegister(AM_REG_CTRL_REG2, (regValue & 0xE4) | 0x1A);
	// Set which interrupts can wake up the device, interrupt active high
	writeRegister(AM_REG_CTRL_REG3, 0x3A);
	// Set data range
	writeRegister(AM_REG_XYZ_DATA_CFG, AM_RANGE_4_G);
	divider = 2048;
	// Set min idle time to sleep. 20 * step of 320ms to give 6.4s of active reading before going back to sleep
	writeRegister(AM_ASLP_COUNT, 20);
	// Clear interrupt
	readRegister(AM_INT_SOURCE);
	// Restore to active mode
	putDeviceInActiveMode;
	setThresholdForMotionDetection(40);
	setAxisForMotionDetection();
	setThresholdForTapDetection(10, 10, 10);
	setTimeLimitForTapDetection();
	setLatencyTimeForTapDetection();
	setAxisForTapDetection();
	enableTapDetection();
	enableMotionDetection();
	enableLandscapeChangeDetection();
	DEBUG.println("Finished setting up acclerometer sensor!");
	return SUCCESS;
}

uint8_t AccelerometerModule::enableInterrupt(uint8_t value)
{
	// Use default INT1 for all interrupts
	uint8_t ctrl4;
	readRegister(AM_REG_CTRL_REG4, &ctrl4);
	writeRegister(AM_REG_CTRL_REG4, ctrl4 | value);
	return SUCCESS;
}

uint8_t AccelerometerModule::read()
{
	// read x y z at once
	Wire.beginTransmission(_i2caddr);
	Wire.write(AM_REG_OUT_X_MSB);
	Wire.endTransmission(false); // MMA8451 + friends uses repeated start!!

	Wire.requestFrom(_i2caddr, 6);
	x = Wire.read();
	x <<= 8;
	x |= Wire.read();
	x >>= 2;
	y = Wire.read();
	y <<= 8;
	y |= Wire.read();
	y >>= 2;
	z = Wire.read();
	z <<= 8;
	z |= Wire.read();
	z >>= 2;
	return SUCCESS;
}

uint8_t AccelerometerModule::alertInterrupt()
{
	hasInterrupt = true;
	return SUCCESS;
}

bool AccelerometerModule::causedWakeUp()
{
	return hasInterrupt;
}

uint8_t AccelerometerModule::setThresholdForTapDetection(uint8_t x_th, uint8_t y_th, uint16_t z_th)
{
	writeRegister(AM_PULSE_THSX, x_th * 1000 / AM_INPUT_SCALE / AM_THRESHOLD_STEP);
	writeRegister(AM_PULSE_THSY, y_th * 1000 / AM_INPUT_SCALE / AM_THRESHOLD_STEP);
	writeRegister(AM_PULSE_THSZ, z_th * 1000 / AM_INPUT_SCALE / AM_THRESHOLD_STEP);
	return SUCCESS;
}

uint8_t AccelerometerModule::setThresholdForMotionDetection(uint8_t th)
{
	writeRegister(AM_REG_FF_MT_THS, th * 1000 / AM_INPUT_SCALE / AM_THRESHOLD_STEP);
	return SUCCESS;
}

uint8_t AccelerometerModule::setTimeLimitForTapDetection(uint8_t time)
{
	putDeviceInStandByMode;
	writeRegister(AM_PULSE_TMLT, time * 2 / 5); // Only for 100Hz
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::setAxisForTapDetection(bool x, bool y, bool z)
{
	putDeviceInStandByMode;
	uint8_t value = 0x40 | (z ? 0x10 : 0) | (y ? 0x04 : 0) | (x ? 0x01 : 0);
	uint8_t config;
	readRegister(AM_PULSE_CFG, &config);
	writeRegister(AM_PULSE_CFG, value | config);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::setAxisForMotionDetection(bool x, bool y, bool z)
{
	putDeviceInStandByMode;
	uint8_t value = 0xC0 | (z ? 0x20 : 0) | (y ? 0x10 : 0) | (x ? 0x08 : 0);
	uint8_t config;
	readRegister(AM_REG_FF_MT_CFG, &config);
	writeRegister(AM_REG_FF_MT_CFG, value | config);
	uint8_t threshold;
	readRegister(AM_REG_FF_MT_THS, &threshold);
	writeRegister(AM_REG_FF_MT_THS, threshold | 0x80);
	writeRegister(AM_REG_FF_MT_COUNT, 50); // Set debound counter to 125ms for Motion detection
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::setLatencyTimeForTapDetection(uint8_t time)
{
	putDeviceInStandByMode;
	writeRegister(AM_PULSE_LTCY, time / 5);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::enableMotionDetection()
{
	putDeviceInStandByMode;
	enableInterrupt(AM_MOTION_INT_BIT);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::enableTapDetection()
{
	putDeviceInStandByMode;
	enableInterrupt(AM_TAP_INT_BIT);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::enableLandscapeChangeDetection()
{
	putDeviceInStandByMode;
	uint8_t data;
	readRegister(AM_REG_PL_CFG, &data);
	writeRegister(AM_REG_PL_CFG, 0xC0 | data);
	writeRegister(AM_REG_PL_COUNT, 50); // Set debounce counter to 125ms (50 * 2.5ms)
	enableInterrupt(AM_PL_INT_BIT);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::enableSleepInterrupt()
{
	putDeviceInStandByMode;
	enableInterrupt(AM_SLEEP_INT_BIT);
	putDeviceInActiveMode;
	return SUCCESS;
}

uint8_t AccelerometerModule::getInterruptSource(uint8_t *origin)
{
	uint8_t source;
	readRegister(AM_INT_SOURCE, &source);
	if (source == 0)
	{
		return FAILED;
	}
	DEBUG.print("Interrupt source reg: ", source);
	uint8_t pl;
	readRegister(AM_REG_PL_STATUS, &pl);
	uint8_t tap;
	readRegister(AM_PULSE_SRC, &tap);
	uint8_t motion;
	readRegister(AM_REG_FF_MT_SRC, &motion);
	uint8_t mode;
	readRegister(AM_REG_SYSMOD, &mode);
	DEBUG.print("System current mode: ", mode);
	if (source & 0x10)
	{
		if (pl & 0x80)
		{
			uint64_t time = millis() % AM_DEBOUNCE_MAX;
			if (time - lastAlertTime > deboundCounterNotification)
			{
				NOTIFIER.alertWarning("Landscape change detected!");
				lastAlertTime = time;
			}
			*origin = AM_PL_INT;
			return SUCCESS;
		}
	}
	else if (source & 0x08)
	{
		uint64_t time = millis() % AM_DEBOUNCE_MAX;
		if (time - lastAlertTime > deboundCounterNotification)
		{
			if (tap & 0x90 == 0x90)
			{
				NOTIFIER.alertWarning("Tap in X direction.");
			}
			else if (tap & 0xC0 == 0xC0)
			{
				NOTIFIER.alertWarning("Tap in Z direction.");
			}
			else if (tap & 0xA0)
			{
				NOTIFIER.alertWarning("Tap in Y direction.");
			}
			else
			{
				NOTIFIER.alertWarning("Should not happen!");
				lastAlertTime = time;
				*origin = AM_ERROR_INT;
				return FAILED;
			}
			lastAlertTime = time;
			*origin = AM_TAP_INT;
			return SUCCESS;
		}
	}
	else if (source & 0x04)
	{
		uint64_t time = millis() % AM_DEBOUNCE_MAX;
		if (time - lastAlertTime > deboundCounterNotification)
		{
			if (!(motion & 0x80))
			{
				NOTIFIER.alertWarning("Error detecting motion!");
				*origin = AM_ERROR_INT;
				lastAlertTime = time;
				return FAILED;
			}
			else if (motion & 0x30)
			{
				NOTIFIER.alertWarning("Motion in Z direction detected!");
			}
			else if (motion & 0x0C)
			{
				NOTIFIER.alertWarning("Motion in Y direction detected");
			}
			else if (motion & 0x03)
			{
				NOTIFIER.alertWarning("Motion in X direction detected");
			}
			lastAlertTime = time;
			*origin = AM_MOTION_INT;
			return SUCCESS;
		}
	}
	else if (source & 0x80)
	{
		DEBUG.println("Sleep/Wake interrupt");
		*origin = AM_SLEEP_INT;
		return SUCCESS;
	}
	else if (source & 1 == 1)
	{
		DEBUG.println("Data ready interrupt");
		*origin = AM_DATA_INT;
		return SUCCESS;
	}
	*origin = AM_ERROR_INT;
	return FAILED;
}

uint8_t AccelerometerModule::printNewData()
{
	read();
	DEBUG.print("New x value: ", (uint16_t)(x * AM_OUTPUT_SCALE / divider), " mg");
	DEBUG.print("New y value: ", (uint16_t)(y * AM_OUTPUT_SCALE / divider), " mg");
	DEBUG.print("New z value: ", (uint16_t)(z * AM_OUTPUT_SCALE / divider), " mg");
}

uint8_t AccelerometerModule::clearInterrupt()
{
	readRegister(AM_INT_SOURCE);
	readRegister(AM_REG_PL_STATUS);
	readRegister(AM_PULSE_SRC);
	readRegister(AM_REG_FF_MT_SRC);
	readRegister(AM_REG_SYSMOD);
	hasInterrupt = false;
	return SUCCESS;
}

uint8_t AccelerometerModule::readVariousRegs()
{
	// Serial.print("AM_REG_SYSMOD: ");
	// Serial.println(readRegisterDebug(AM_REG_SYSMOD) & 0x03, BIN);
	// Serial.print("AM_PULSE_CFG: ");
	// Serial.println(readRegisterDebug(AM_PULSE_CFG), BIN);
	// Serial.print("AM_REG_PL_CFG: ");
	// Serial.println(readRegisterDebug(AM_REG_PL_CFG), BIN);
	// Serial.print("AM_REG_FF_MT_CFG: ");
	// Serial.println(readRegisterDebug(AM_REG_FF_MT_CFG), BIN);
	// Serial.print("AM_REG_CTRL_REG1: ");
	// Serial.println(readRegisterDebug(AM_REG_CTRL_REG1), BIN);
	// Serial.print("AM_REG_CTRL_REG2: ");
	// Serial.println(readRegisterDebug(AM_REG_CTRL_REG2), BIN);
	// Serial.print("AM_REG_CTRL_REG3: ");
	// Serial.println(readRegisterDebug(AM_REG_CTRL_REG3), BIN);
	// Serial.print("AM_REG_CTRL_REG4: ");
	// Serial.println(readRegisterDebug(AM_REG_CTRL_REG4), BIN);
	// Serial.print("AM_REG_CTRL_REG5: ");
	// Serial.println(readRegisterDebug(AM_REG_CTRL_REG5), BIN);
	// Serial.print("AM_PULSE_THSX: ");
	// Serial.println(readRegisterDebug(AM_PULSE_THSX), BIN);
	// Serial.print("AM_PULSE_THSY: ");
	// Serial.println(readRegisterDebug(AM_PULSE_THSY), BIN);
	// Serial.print("AM_PULSE_THSZ: ");
	// Serial.println(readRegisterDebug(AM_PULSE_THSZ), BIN);
	// Serial.print("AM_ASLP_COUNT: ");
	// Serial.println(readRegisterDebug(AM_ASLP_COUNT), BIN);
	// Serial.print("AM_REG_PL_THS: ");
	// Serial.println(readRegisterDebug(AM_REG_PL_THS), BIN);
	// Serial.print("AM_REG_FF_MT_COUNT: ");
	// Serial.println(readRegisterDebug(AM_REG_FF_MT_COUNT), BIN);
	// Serial.print("AM_REG_PL_COUNT: ");
	// Serial.println(readRegisterDebug(AM_REG_PL_COUNT), BIN);
	// Serial.print("AM_PULSE_LTCY: ");
	// Serial.println(readRegisterDebug(AM_PULSE_LTCY), BIN);
	// Serial.print("AM_INT_SOURCE: ");
	// Serial.println(readRegisterDebug(AM_INT_SOURCE), BIN);
	// Serial.print("AM_REG_PL_STATUS: ");
	// Serial.println(readRegisterDebug(AM_REG_PL_STATUS), BIN);
	// Serial.print("AM_PULSE_SRC: ");
	// Serial.println(readRegisterDebug(AM_PULSE_SRC), BIN);
	// Serial.print("AM_REG_FF_MT_SRC: ");
	// Serial.println(readRegisterDebug(AM_REG_FF_MT_SRC), BIN);
	return SUCCESS;
}

uint8_t AccelerometerModule::getOrientation(uint8_t *orientation)
{
	uint8_t status;
	readRegister(AM_REG_PL_STATUS, &status);
	*orientation = status & 0x07;
	return SUCCESS;
}

uint8_t AccelerometerModule::setRange(uint8_t range)
{
	uint8_t reg1;
	readRegister(AM_REG_CTRL_REG1, &reg1);
	writeRegister(AM_REG_CTRL_REG1, 0x00); // deactivate
	writeRegister(AM_REG_XYZ_DATA_CFG, range & 0x3);
	writeRegister(AM_REG_CTRL_REG1, reg1 | 0x01); // activate
	return SUCCESS;
}

uint8_t AccelerometerModule::getRange(uint8_t *range)
{
	uint8_t config;
	readRegister(AM_REG_XYZ_DATA_CFG, &config);
	*range = config & 0x03;
	return SUCCESS;
}

uint8_t AccelerometerModule::setDataRate(uint8_t dataRate)
{
	divider = 1 << (12 - dataRate);
	uint8_t ctl1;
	readRegister(AM_REG_CTRL_REG1, &ctl1);
	writeRegister(AM_REG_CTRL_REG1, 0x00); // deactivate
	ctl1 &= ~(AM_DATARATE_MASK << 3);	  // mask off bits
	ctl1 |= (dataRate << 3);
	writeRegister(AM_REG_CTRL_REG1, ctl1 | 0x01); // activate
	return SUCCESS;
}

uint8_t AccelerometerModule::getDataRate(uint8_t *dataRate)
{
	uint8_t ctl1;
	readRegister(AM_REG_CTRL_REG1, &ctl1);
	*dataRate = (ctl1 >> 3) & AM_DATARATE_MASK;
	return SUCCESS;
}

uint8_t AccelerometerModule::setDeboundRate(uint8_t rate)
{
	deboundCounterNotification = rate;
	return SUCCESS;
}

uint8_t AccelerometerModule::getDeboundRate(uint8_t *rate)
{
	*rate = deboundCounterNotification;
	return SUCCESS;
}
