#ifndef LOCK_H
#define LOCK_H

#include "Notification.h"
#include "MotorModule.h"

extern Notification NOTIFIER;

using callback_t = void (*)(void);

class Lock
{
  private:
	class MotorModule *motorModule;
	bool isLocked;
	callback_t openCallback;
	callback_t closeCallback;

  public:
	Lock(class MotorModule *m);
	void openIfTrue(bool condition);
	bool isOpened();
	uint8_t openWithKey();
	uint8_t toggleState();
	uint8_t addEventListener(callback_t openCallback, callback_t closeCallback);
};

#endif
