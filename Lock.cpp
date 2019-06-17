#include "Lock.h"

Lock::Lock(class Notification *n, class MotorModule *m) : notifier(n), motorModule(m)
{
	this->motorModule->turnToClosePosition();
}

void Lock::openIfTrue(bool condition)
{
	if (condition)
	{
		this->motorModule->turnToOpenPosition();
		this->notifier->alertSuccess();
	}
	else
	{
		this->notifier->alertFailure();
	}
}