#ifndef LOCK_H
#define LOCK_H

#include "Notification.h"
#include "MotorModule.h"

class Lock
{
  private:
	class MotorModule *motorModule;
	bool isLocked;

  public:
	Lock(class MotorModule *m);
	void openIfTrue(bool condition);
	bool isOpened();
};

#endif
