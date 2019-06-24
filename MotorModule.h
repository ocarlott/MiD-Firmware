#ifndef MOTORMODULE_H
#define MOTORMODULE_H

#include <Servo.h>
#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

class MotorModule
{
  private:
	class Servo motor;
	bool started;

  public:
	MotorModule();
	uint8_t setup();
	uint8_t getAngle(uint8_t *angle);
	uint8_t turnTo(uint8_t degree);
	uint8_t turnToOpenPosition();
	uint8_t turnToClosePosition();
};

#endif
