#ifndef MOTORMODULE_H
#define MOTORMODULE_H

#include <variant.h>
#include <HardwarePWM.h>
#include <Servo.h>
#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

extern Debug DEBUG;

class MotorModule
{
  private:
	class Servo *motor;

  public:
	MotorModule();
	~MotorModule();
	uint8_t getAngle(uint8_t *angle);
	uint8_t turnTo(uint8_t degree);
	uint8_t turnToOpenPosition();
	uint8_t turnToClosePosition();
};

#endif
