#define NRF32
#define ARDUINO_ARCH_NRF52
#include <Arduino.h>
#include "Debug.h"
#include "Notification.h"
#include "KeypadModule.h"
#include "Constant.h"
#include "MotorModule.h"
#include "AccelerometerModule.h"
#include "FingerprintModule.h"
#include "Storage.h"
#include "SwitchModule.h"

Debug DEBUG;
Notification NOTIFIER;

Storage storage;
MotorModule motorModule;
Lock lock(&motorModule);
KeypadModule kpm(&storage, &lock);
AccelerometerModule am;
FingerprintModule fpm(&storage, &lock);
SwitchModule sm(&lock);

void callbackOnClosed(void)
{
  DEBUG.println("Close callback called!");
  kpm.enable();
  kpm.handleKey();
  fpm.enable();
  am.enable();
  am.clearInterrupt();
}

void callbackOnOpened(void)
{
  DEBUG.println("Open callback called!");
	kpm.disable();
  fpm.disable();
  am.disable();
}

void setup()
{
	Serial.begin(SERIAL_FREQ);
	DEBUG.enable();
	NOTIFIER.setup();
	storage.setup();
	am.setup();
	fpm.setup();
	kpm.setup();
	sm.setup();
	lock.addEventListener(callbackOnOpened, callbackOnClosed);
	Serial.println("Done setup!");
}

void loop()
{
	DEBUG.flushDebugMessages();
	//  sleep_enable();
	//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//  sleep_cpu();
	if (lock.isOpened())
	{
	}
	else
	{
		if (am.isReady())
		{
			am.handleInterrupt();
			am.printNewData();
			am.clearInterrupt();
		};
    
		if (kpm.isReady())
		{
			kpm.handleKey();
		};

		if (fpm.isReady())
		{
			fpm.run();
		};
	}
	if (sm.isReady())
	{
		sm.run();
	}
}
