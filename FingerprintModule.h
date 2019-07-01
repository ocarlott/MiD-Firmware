#ifndef FINGERPRINTMODULE_H
#define FINGERPRINTMODULE_H

#include "Constant.h"
#include <Arduino.h>
#include "Storage.h"
#include "Lock.h";
#include "AdafruitFingerprint.h"
#include "Debug.h"
#include "Notification.h"

extern Debug DEBUG;
extern Notification NOTIFIER;

class FingerprintModule
{
  private:
	class AdafruitFingerprint *reader;
	class Storage *storage;
	class Lock *lock;
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

  public:
	FingerprintModule(class Storage *s, class Lock *l);
	uint8_t setup();
	uint8_t run();
	uint8_t addFingerprint();
	bool isReady();
	uint8_t disable();
	uint8_t enable();
};

#endif
