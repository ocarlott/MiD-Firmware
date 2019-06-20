#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <avr/sleep.h>
#include "Notification.h"
#include "KeypadModule.h"
#include "EventHandler.h"
#include "Constant.h"
#include "MotorModule.h"
#include "Storage.h"
#include "Debug.h"

Storage storage;
Notification notifier;
MotorModule motorModule;
Lock lock(&notifier, &motorModule);
KeypadModule kpm(&storage, &lock, &notifier);

void setup()
{
	Serial.begin(SERIAL_FREQ);
	Debug::enable();
	notifier.begin();
	kpm.begin();
	EventHandler::setup(&kpm);
	kpm.setup(EventHandler::keypadEventHandler);
}

void loop()
{
	//  sleep_enable();
	//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//  sleep_cpu();
	kpm.getKey();
}
