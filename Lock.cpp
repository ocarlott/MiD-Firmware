#include "Lock.h"

Lock::Lock(class Notification *n, class MotorModule *m) : notifier(n), motorModule(m)
{
	this->isLocked = true;
}

void Lock::openIfTrue(bool condition)
{
	if (condition)
	{
		this->motorModule->turnToOpenPosition();
		this->isLocked = false;
		this->notifier->alertSuccess();
	}
	else
	{
		this->notifier->alertFailure();
	}
}

bool Lock::isOpened()
{
	return !this->isLocked;
}