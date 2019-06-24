#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include "Constant.h"
#include "Storage.h"
#include "Lock.h"
#include <Keypad.h>
#include <Wire.h>
#include <Keypad_MC17.h>
#include "Type.h"

class KeypadModule
{

  private:
	unsigned long lastTime;
	unsigned long currentIndex;
	bool enabled;
	class Keypad_MC17 keypad;
	class Storage *storage;
	class Lock *lock;
	bool startCapturing;
	char passcode[PASSCODE_LENGTH];

  public:
	KeypadModule(class Storage *s, class Lock *l);
	uint8_t setup(void (*handler)(char));
	uint8_t handleKey(char key);
	uint8_t begin();
	uint8_t enable();
	uint8_t disable();
	uint8_t getComputedPasscode(uint32_t *passcode);
	uint8_t getKey();
};

#endif
