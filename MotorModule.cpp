#include "MotorModule.h"

MotorModule::MotorModule()
{
	this->motor.attach(PIN_MOTOR);
}

void MotorModule::turnToOpenPosition()
{
	this->motor.write(180);
}

void MotorModule::turnToClosePosition()
{
	this->motor.write(0);
}