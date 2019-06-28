#include "MotorModule.h"

MotorModule::MotorModule()
{
	this->started = false;
}

uint8_t MotorModule::setup()
{
//	this->turnTo(0);
	DEBUG.println("Finished setting up motor!");
	return SUCCESS;
}

uint8_t MotorModule::turnToOpenPosition()
{
	DEBUG.println("Turn motor to opened position!");
	this->turnTo(180);
	return SUCCESS;
}

uint8_t MotorModule::turnToClosePosition()
{
	DEBUG.println("Turn motor to closed position");
	this->turnTo(0);
	return SUCCESS;
}

uint8_t MotorModule::getAngle(uint8_t *angle)
{
	*angle = this->motor.read();
	return SUCCESS;
}

uint8_t MotorModule::turnTo(uint8_t degree)
{
	this->motor.attach(PIN_MOTOR);
	uint8_t current;
	this->getAngle(&current);
	if (current > degree)
	{
		for (uint8_t i = current; i > degree; i--)
		{
			this->motor.write(i);
			delay(15);
		}
	}
	else
	{
		for (uint8_t i = current; i < degree; i++)
		{
			this->motor.write(i);
			delay(15);
		}
	}
	this->motor.detach();
	return SUCCESS;
}
