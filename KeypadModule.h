#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include "Constant.h"
#include "Storage.h"
#include "Lock.h"
#include <Keypad.h>

struct KeyCode
{
	int id;
	int code;
};

struct KeyCodeManager
{
	int numberOfKeyCodes;
	int id;
	struct KeyCode codes[PASSCODE_MAX_COUNT];
};

class KeypadModule
{

  private:
	int lastTime;
	int currentIndex;
	bool enabled;
	class Keypad keypad;
	class Storage *storage;
	class Lock *lock;
	char passcode[PASSCODE_LENGTH];

  public:
	KeypadModule(class Storage *s, class Lock *l);
	void setup(void (*handler)(char));
	void handleKey(KeypadEvent key);
	void enable();
	void disable();
	int getComputedPasscode();
	void getKey();
};

#endif