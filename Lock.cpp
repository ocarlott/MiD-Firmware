#include "Lock.h"

Lock::Lock(class MotorModule *m) : motorModule(m)
{
	this->isLocked = true;
}

void Lock::openIfTrue(bool condition)
{
	if (condition)
	{
		this->motorModule->turnToOpenPosition();
		this->isLocked = false;
		NOTIFIER.alertSuccess("Opening the door!");
	}
	else
	{
		NOTIFIER.alertFailure("Wrong credentials!");
	}
}

bool Lock::isOpened()
{
	return !this->isLocked;
}

uint8_t Lock::toggleState()
{
	if (this->isLocked)
	{
		this->motorModule->turnToOpenPosition();
		this->isLocked = false;
		NOTIFIER.alertSuccess("Opened with back button!");
	}
	else
	{
		this->motorModule->turnToClosePosition();
		this->isLocked = true;
		NOTIFIER.alertSuccess("Closed with back button!");
	}
}