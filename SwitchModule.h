#ifndef SWITCHMODULE_H
#define SWITCHMODULE_H

#include <Arduino.h>
#include "KeypadModule.h"
#include "Lock.h"
#include "Notification.h"

extern Notification NOTIFIER;

class SwitchModule
{
  public:
	SwitchModule(class Lock *l);
	uint8_t setup();
  bool isReady();
  uint8_t run();
	uint8_t enableFrontSwitch();
	uint8_t enableBackButton();
	uint8_t disableFrontSwitch();
	uint8_t disableBackButton();

  private:
	uint8_t frontSwitchEventHandler();
	uint8_t backButtonEventHandler();
	static void frontSwitchISR();
	static void backButtonISR();
	static volatile bool frontSwitchTriggered;
	static volatile bool backButtonPressed;
	class Lock *lock;
};

#endif
