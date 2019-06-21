#include <Arduino.h>
#include <avr/sleep.h>
#include "Notification.h"
#include "KeypadModule.h"
#include "EventHandler.h"
#include "Constant.h"
#include "MotorModule.h"
#include "AccelerometerModule.h"
#include "FingerprintModule.h"
#include "Storage.h"
#include "Debug.h"

Storage storage;
Notification notifier;
MotorModule motorModule;
Lock lock(&notifier, &motorModule);
KeypadModule kpm(&storage, &lock, &notifier);
AccelerometerModule am(&notifier);
FingerprintModule fpm(&storage, &notifier);

void setup()
{
	Serial.begin(SERIAL_FREQ);
	Debug::enable();
	notifier.setup();
	motorModule.setup();
	am.setup();
	fpm.setup();
	kpm.begin();
	EventHandler::setup(&kpm);
	kpm.setup(EventHandler::keypadEventHandler);
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
		Debug::print("Loop ended!");
	}
}
