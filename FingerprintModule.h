#ifndef FINGERPRINTMODULE_H
#define FINGERPRINTMODULE_H

// #include <finger.h>
#include "Constant.h"
#include <Arduino.h>
#include "Storage.h"
#include "Debug.h"
#include "Notification.h"
#include "Lock.h";
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// class FingerprintModule
// {
//   public:
// 	FingerprintModule();
// 	void poll();
// };

class FingerprintModule
{
  private:
	class Adafruit_Fingerprint *reader;
	class Storage *storage;
	class Lock *lock;
	class Notification *notifier;
	static volatile bool ready;
	bool enrollmentRequested;
	uint16_t waitTimeForCheckingFingerprint;
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
	FingerprintModule(class Storage *s, class Notification *n, class Lock *l);
	uint8_t setup();
	uint8_t run();
	uint8_t addFingerprint();
	bool isReady();
};

#endif