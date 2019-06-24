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
		NOTIFIER.alertSuccess();
	}
	else
	{
		NOTIFIER.alertFailure();
	}
}

bool Lock::isOpened()
{
	return !this->isLocked;
}
