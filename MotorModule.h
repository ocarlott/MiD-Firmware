#ifndef MOTORMODULE_H
#define MOTORMODULE_H

#include <Servo.h>
#include "Constant.h"

class MotorModule
{
  private:
	class Servo motor;

  public:
	MotorModule();
	void turnToOpenPosition();
	void turnToClosePosition();
};

#endif