#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include <Arduino.h>
#include "Constant.h"
#include "WiredDevice.h"

class Debug;
class Storage;
class Lock;

class KeypadModule : public WiredDevice
{

  private:
	unsigned long lastTime;
	unsigned long currentIndex;
	bool enabled;
	char passcode[PASSCODE_LENGTH];
	static void isr();
	static volatile bool ready;
	volatile bool startEnteringPasscode;
	class Debug *DEBUG;
	class Storage *STORAGE;
	class Lock *LOCK;

  public:
	KeypadModule();
	uint8_t setup(class Debug *d, class Storage *st, class Lock *l);
	uint8_t handleKey();
	uint8_t enable();
	uint8_t disable();
	uint8_t getComputedPasscode(uint32_t *passcode);
	uint8_t getKey(uint8_t *data);
	bool isReady();
};

#endif
