#ifndef ACCELEROMETERMODULE_H
#define ACCELEROMETERMODULE_H

#include <Wire.h>
#include "Constant.h"
#include "WiredDevice.h"
#include <Arduino.h>
#include "Debug.h"
#include "Notification.h"

extern Debug DEBUG;
extern Notification NOTIFIER;

class AccelerometerModule : WiredDevice
{
  private:
	class Notification *notifier;
	uint8_t thresholdForTap;
	uint8_t thresholdForMotion;
	uint16_t deboundCounterNotification;
	int8_t _i2caddr;
	uint16_t divider;
	volatile bool hasInterrupt;
	uint64_t lastAlertTime;
	int16_t x, y, z;
	uint8_t enableInterrupt(uint8_t value);

  public:
	AccelerometerModule();
	uint8_t read();
	uint8_t setup(uint8_t addr = AM_DEFAULT_ADDRESS);
	uint8_t setThresholdForTapDetection(uint8_t x_th, uint8_t y_th, uint16_t z_th); // 0 - 100 (0 - 5g)
	uint8_t setAxisForTapDetection(bool x = true, bool y = true, bool z = true);
	uint8_t setThresholdForMotionDetection(uint8_t threshold); // 0 - 100 (coresponding for 0 to 5g)
	uint8_t setAxisForMotionDetection(bool x = true, bool y = true, bool z = true);
	uint8_t setTimeLimitForTapDetection(uint8_t time = 500);	// Max time 638ms for 100Hz
	uint8_t setLatencyTimeForTapDetection(uint8_t time = 1200); // Max 1276ms for 100Hz
	uint8_t enableLandscapeChangeDetection();
	uint8_t enableTapDetection();
	uint8_t enableMotionDetection();
	uint8_t enableSleepInterrupt();
	bool isTapDetectedInXAxis();
	uint8_t getInterruptSource(uint8_t *origin);
	uint8_t printNewData();
	uint8_t alertInterrupt();
	bool causedWakeUp();
	uint8_t clearInterrupt();
	uint8_t readVariousRegs();
	uint8_t getOrientation(uint8_t *orientation);
	uint8_t setRange(uint8_t range);
	uint8_t getRange(uint8_t *range);
	uint8_t setDataRate(uint8_t dataRate);
	uint8_t getDataRate(uint8_t *dataRate);
	uint8_t setDeboundRate(uint8_t rate);
	uint8_t getDeboundRate(uint8_t *rate);
};

#endif
