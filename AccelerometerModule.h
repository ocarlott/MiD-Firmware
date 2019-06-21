#ifndef ACCELEROMETERMODULE_H
#define ACCELEROMETERMODULE_H

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "Debug.h"
#include "Constant.h"
#include "Notification.h"
#include <math.h>

class AccelerometerModule
{
  private:
	static uint8_t timerForOrientationReading;
	class Adafruit_MMA8451 mma;
	class Notification *notifier;
	uint16_t originalValue;
	uint16_t originalOrientation;
	uint16_t threshold;
	uint8_t deboundCounter;
	uint8_t deboundRate;

  public:
	AccelerometerModule(class Notification *);
	uint8_t setup();
	uint8_t read();
	uint8_t getThreshold(uint16_t *threshold);
	uint8_t setThreshold(uint16_t threshold);
	uint8_t setDeboundRate(uint8_t rate);
	uint8_t getDeboundRate(uint8_t *rate);
	uint8_t calibrateOrientation();
};

#endif