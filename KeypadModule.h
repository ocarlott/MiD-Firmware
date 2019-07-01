#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include "Constant.h"
#include "Storage.h"
#include "Lock.h"
#include "Type.h"
#include "Debug.h"
#include "WiredDevice.h"

extern Debug DEBUG;

class KeypadModule : public WiredDevice
{

  private:
	unsigned long lastTime;
	unsigned long currentIndex;
	bool enabled;
	class Storage *storage;
	class Lock *lock;
	bool startCapturing;
	char passcode[PASSCODE_LENGTH];
	static void isr();
	static volatile bool ready;

  public:
	KeypadModule(class Storage *s, class Lock *l);
	uint8_t setup();
	uint8_t handleKey();
	uint8_t enable();
	uint8_t disable();
	uint8_t getComputedPasscode(uint32_t *passcode);
	uint8_t getKey(uint8_t *data);
	bool isReady();
};

#endif
