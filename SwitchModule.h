#ifndef SWITCHMODULE_H
#define SWITCHMODULE_H

#include <Arduino.h>
#include "Constant.h"

class BluetoothModule;
class Debug;
class Lock;

class SwitchModule
{
  public:
	SwitchModule();
	uint8_t setup(class Debug *d, class BluetoothModule *b, class Lock *l);
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
	class BluetoothModule *BLE;
	class Debug *DEBUG;
	class Lock *LOCK;
	static void frontSwitchISR();
	static void backButtonISR();
	static void bluetoothButtonISR();
	static volatile bool frontSwitchTriggered;
	static volatile bool backButtonPressed;
	static volatile bool bluetoothButtonPressed;
};

#endif
