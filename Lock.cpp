#include "Lock.h"
#include "Debug.h"
#include "MotorModule.h"
#include "Notification.h"

uint8_t Lock::setup(class Debug *d, class Notification *n, class MotorModule *m)
{
	DEBUG = d;
	NOTIFIER = n;
	MOTOR = m;
	this->isLocked = true;
}

void Lock::openIfTrue(bool condition)
{
	if (condition)
	{
		MOTOR->turnToOpenPosition();
		this->isLocked = false;
		this->openCallback();
		NOTIFIER->alertSuccess("Opening the door!");
	}
	else
	{
		NOTIFIER->alertFailure("Wrong credentials!");
	}
};

bool Lock::isOpened()
{
	return !this->isLocked;
};

uint8_t Lock::toggleState()
{
	if (this->isLocked)
	{
		MOTOR->turnToOpenPosition();
		this->isLocked = false;
		this->openCallback();
		NOTIFIER->alertSuccess("Opened with back button!");
	}
	else
	{
		MOTOR->turnToClosePosition();
		this->isLocked = true;
		this->closeCallback();
		NOTIFIER->alertSuccess("Closed with back button!");
	}
};

uint8_t Lock::openWithKey()
{
	this->isLocked = false;
	this->openCallback();
	return SUCCESS;
};

uint8_t Lock::addEventListener(callback_t oC, callback_t cC)
{
	this->openCallback = oC;
	this->closeCallback = cC;
}

uint8_t Lock::lock()
{
	if (!this->isLocked)
	{
		MOTOR->turnToClosePosition();
		this->isLocked = true;
		this->closeCallback();
	}
}
