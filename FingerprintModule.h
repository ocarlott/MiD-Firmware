#ifndef FINGERPRINTMODULE_H
#define FINGERPRINTMODULE_H

#include <Arduino.h>
#include "Constant.h"
#include "AdafruitFingerprint.h"

class Debug;
class Notification;
class Lock;
class Storage;

class FingerprintModule
{
  private:
	class AdafruitFingerprint *reader;
	static volatile bool ready;
	bool enrollmentRequested;
	uint16_t waitTimeForCheckingFingerprint;
	bool enabled;
	static void isr();
	uint8_t start();
	uint8_t stop();
	uint8_t enroll(uint8_t *id);
	uint8_t check(uint8_t *id);
	uint8_t getImage(unsigned long waitTime);
	uint8_t convertImage();
	uint8_t waitForFingerRemoved();
	uint8_t createModel();
	uint8_t storeModel(uint8_t id);
	uint8_t search(uint8_t *id);
	class Debug *DEBUG;
	class Storage *STORAGE;
	class Lock *LOCK;
	class Notification *NOTIFIER;

  public:
	FingerprintModule();
	uint8_t setup(class Debug *d, class Storage *st, class Lock *l, class Notification *n);
	uint8_t run();
	uint8_t addFingerprint();
	bool isReady();
	uint8_t disable();
	uint8_t enable();
};

#endif
