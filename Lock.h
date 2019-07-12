#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>
#include "Constant.h"

class Debug;
class Notification;
class MotorModule;

using callback_t = void (*)(void);

class Lock
{
  private:
	bool isLocked;
	callback_t openCallback;
	callback_t closeCallback;
	class Debug *DEBUG;
	class Notification *NOTIFIER;
	class MotorModule *MOTOR;

  public:
	uint8_t setup(class Debug *d, class Notification *n, class MotorModule *m);
	void openIfTrue(bool condition);
	bool isOpened();
	uint8_t openWithKey();
	uint8_t toggleState();
	uint8_t addEventListener(callback_t openCallback, callback_t closeCallback);
	uint8_t lock();
};

#endif
