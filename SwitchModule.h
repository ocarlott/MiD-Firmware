#ifndef SWITCHMODULE_H
#define SWITCHMODULE_H

#include <Arduino.h>
#include "KeypadModule.h"
#include "Lock.h"
#include "Notification.h"
#include "BluetoothModule.h"

extern Notification NOTIFIER;

class SwitchModule
{
  public:
	SwitchModule(class Lock *l, class BluetoothModule *b);
	uint8_t setup();
	bool isReady();
	uint8_t run();
	uint8_t enableFrontSwitch();
	uint8_t enableBackButton();
	uint8_t disableFrontSwitch();
	uint8_t disableBackButton();
	uint8_t enableBluetoothButton();
	uint8_t disableBluetoothButton();

  private:
	uint8_t frontSwitchEventHandler();
	uint8_t backButtonEventHandler();
	uint8_t bluetoothButtonHandler();
	static void frontSwitchISR();
	static void backButtonISR();
	static void bluetoothButtonISR();
	static volatile bool frontSwitchTriggered;
	static volatile bool backButtonPressed;
	static volatile bool bluetoothButtonPressed;
	class Lock *lock;
	class BluetoothModule *blm;
};

#endif
