#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <avr/sleep.h>
#include "KeypadModule.h"
#include "EventHandler.h"
#include "Constant.h"
#include "MotorModule.h"
#include "Debug.h"

enum Status
{
	Success,
	Failure,
	Waiting,
	Error
};

Storage storage;
Notification notifier;
MotorModule motorModule;
Lock lock(&notifier, &motorModule);
KeypadModule kpm(&storage, &lock);

void setup()
{
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
