#define NRF32
#include <Arduino.h>
#include "Debug.h"
#include "Notification.h"
#include "KeypadModule.h"
#include "Constant.h"
#include "MotorModule.h"
#include "AccelerometerModule.h"
#include "FingerprintModule.h"
#include "Storage.h"

static Debug DEBUG;
static Notification NOTIFIER;

Storage storage;
MotorModule motorModule;
Lock lock(&motorModule);
KeypadModule kpm(&storage, &lock);
AccelerometerModule am;
FingerprintModule fpm(&storage, &lock);

void keypadEventHandler(KeypadEvent key)
{
	kpm.handleKey(key);
};

void setup()
{
	Serial.begin(SERIAL_FREQ);
	Serial.println("Start setting up.");
	DEBUG.enable();
	NOTIFIER.setup();
  storage.setup();
	motorModule.setup();
	am.setup();
	fpm.setup();
	kpm.begin();
	kpm.setup(keypadEventHandler);
  Serial.println("Got here!");
}

void loop()
{
	//  sleep_enable();
	//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//  sleep_cpu();
	if (lock.isOpened())
	{
	}
	else
	{
		kpm.getKey();
		am.read();
		if (fpm.isReady())
		{
			fpm.run();
		}
//		DEBUG.println("Loop ended!");
	}
}
