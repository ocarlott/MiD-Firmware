#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

#include <Arduino.h>
#include "KeypadModule.h"
#include "Lock.h"
#include "Notification.h"

extern KeypadModule kpm;
extern Notification NOTIFIER;
extern Lock lock;

void keypadEventHandler(KeypadEvent key)
{
	kpm.handleKey(key);
};

void setupSwitches()
{
	pinMode(PIN_BACK_BUTTON, INPUT_PULLUP);
	pinMode(PIN_FRONT_SWITCH, INPUT_PULLUP);
}

void frontSwitchEventHandler()
{
}

void enableFrontSwitch()
{
	attachInterrupt(digitalPinToInterrupt(PIN_FRONT_SWITCH), frontSwitchEventHandler, RISING);
}

void disableFrontSwitch()
{
	detachInterrupt(digitalPinToInterrupt(PIN_FRONT_SWITCH));
}

void backButtonEventHandler()
{
}

void enableBackButton()
{
	attachInterrupt(digitalPinToInterrupt(PIN_BACK_BUTTON), backButtonEventHandler, RISING);
}

void disableBackButton()
{
	detachInterrupt(digitalPinToInterrupt(PIN_BACK_BUTTON));
}

#endif