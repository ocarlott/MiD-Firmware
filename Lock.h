#ifndef LOCK_H
#define LOCK_H

#include "Notification.h"
#include "MotorModule.h"

class Lock
{
  private:
	class Notification *notifier;
	class MotorModule *motorModule;
	bool isLocked;

  public:
	Lock(class Notification *n, class MotorModule *m);
	void openIfTrue(bool condition);
	bool isOpened();
};

#endif