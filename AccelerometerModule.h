#ifndef ACCELEROMETERMODULE_H
#define ACCELEROMETERMODULE_H

#include <Arduino.h>
#include "Constant.h"
#include "WiredDevice.h"

class Debug;
class Notification;

class AccelerometerModule : public WiredDevice
{
  private:
	uint8_t thresholdForKnock;
	uint8_t thresholdForMotion;
	uint16_t deboundCounterNotification;
	uint16_t divider;
	volatile static bool ready;
	uint64_t lastAlertTime;
	bool enabled;
	int16_t x, y, z;
	uint8_t enableInterrupt(uint8_t value);
	class Debug *DEBUG;
	class Notification *NOTIFIER;

  public:
	AccelerometerModule();
	uint8_t read();
	uint8_t enable();
	uint8_t disable();
	uint8_t setup(class Debug *d, class Notification *n, uint8_t addr = AM_DEFAULT_ADDRESS);
	uint8_t setThresholdForKnockDetection(uint8_t x_th, uint8_t y_th, uint16_t z_th); // 0 - 100 (0 - 4g)
	uint8_t setAxisForKnockDetection(bool x = true, bool y = true, bool z = true);
	uint8_t setThresholdForMotionDetection(uint8_t threshold); // 0 - 100 (coresponding for 0 to 4g)
	uint8_t setAxisForMotionDetection(bool x = true, bool y = true, bool z = true);
	uint8_t setTimeLimitForKnockDetection(uint8_t time = 160);	// Max 5.1s
	uint8_t setTimeWindowForSecondKnock(uint8_t time = 2400);	 // Max 10.2s
	uint8_t setLatencyTimeForKnockDetection(uint8_t time = 2000); // Max 10.2s
	uint8_t enableLandscapeChangeDetection();
	uint8_t enableKnockDetection();
	uint8_t enableMotionDetection();
	uint8_t enableSleepInterrupt();
	uint8_t handleInterrupt(uint8_t *origin = NULL);
	uint8_t printNewData();
	void static isr();
	bool isReady();
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
