#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include "Constant.h"
#include "Storage.h"
#include "Lock.h"
#include <Keypad.h>
#include <Wire.h>
#include "Notification.h"
#include <Keypad_MC17.h>
#include "Type.h"

class KeypadModule
{

  private:
	int lastTime;
	int currentIndex;
	bool enabled;
	class Keypad_MC17 keypad;
	class Storage *storage;
	class Lock *lock;
	class Notification *notifier;
	bool startCapturing;
	char passcode[PASSCODE_LENGTH];

  public:
	KeypadModule(class Storage *s, class Lock *l, class Notification *n);
	void setup(void (*handler)(char));
	void handleKey(char key);
	void begin();
	void enable();
	void disable();
	int getComputedPasscode();
	void getKey();
};

#endif