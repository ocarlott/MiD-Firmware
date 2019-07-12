#ifndef MOTORMODULE_H
#define MOTORMODULE_H

#include <Arduino.h>
#include "Constant.h"
#include <HardwarePWM.h>
#include <Servo.h>

class Debug;

class MotorModule
{
  private:
	class Servo *motor;
	class Debug *DEBUG;

  public:
	MotorModule();
	~MotorModule();
	uint8_t setup(class Debug *d);
	uint8_t getAngle(uint8_t *angle);
	uint8_t turnTo(uint8_t degree);
	uint8_t turnToOpenPosition();
	uint8_t turnToClosePosition();
};

#endif
