#include "MotorModule.h"

MotorModule::MotorModule()
{
	this->motor = new Servo();
}

uint8_t MotorModule::turnToOpenPosition()
{
	DEBUG.println("Turn motor to opened position!");
	this->turnTo(110);
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
	*angle = this->motor->read();
	return SUCCESS;
}

uint8_t MotorModule::turnTo(uint8_t degree)
{
  this->motor->attach(PIN_MOTOR);
//  this->motor->write(degree);
//  delay(50);
	uint8_t start = degree == 0 ? 110 : 0;
  uint8_t step = degree == 0 ? -1 : 1;
	for (uint8_t i = start; i != degree; i += step)
  {
    this->motor->write(i);
    delay(15);
  }
	this->motor->detach();
	return SUCCESS;
}

MotorModule::~MotorModule()
{
	delete this->motor;
}
