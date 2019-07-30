#include "SwitchModule.h"

volatile bool SwitchModule::frontSwitchTriggered;
volatile bool SwitchModule::backButtonPressed;
volatile bool SwitchModule::bluetoothButtonPressed;

SwitchModule::SwitchModule(class Lock *l, class BluetoothModule *b) : lock(l), blm(b)
{
	SwitchModule::backButtonPressed = false;
	SwitchModule::frontSwitchTriggered = false;
	SwitchModule::bluetoothButtonPressed = false;
}

uint8_t SwitchModule::setup()
{
	pinMode(PIN_BACK_BUTTON, INPUT_PULLUP);
	pinMode(PIN_FRONT_SWITCH, INPUT_PULLUP);
	pinMode(PIN_BLUETOOTH_BUTTON, INPUT_PULLUP);
//	enableFrontSwitch();
	enableBackButton();
	enableBluetoothButton();
	return SUCCESS;
}

bool SwitchModule::isReady()
{
	return SwitchModule::frontSwitchTriggered || SwitchModule::backButtonPressed || SwitchModule::bluetoothButtonPressed;
}

uint8_t SwitchModule::run()
{
	if (SwitchModule::frontSwitchTriggered)
	{
		this->frontSwitchEventHandler();
	}
	else if (SwitchModule::backButtonPressed)
	{
		this->backButtonEventHandler();
	}
	else if (SwitchModule::bluetoothButtonPressed)
	{
		this->bluetoothButtonHandler();
	}
}

uint8_t SwitchModule::checkFrontSwitch()
{
  uint8_t state = digitalRead(PIN_FRONT_SWITCH);
  if (state == LOW && lock->isOpened())
  {
    lock->openWithKey();
  }
  else if (state == HIGH && !lock->isOpened())
  {
    lock->closeWithKey();
  }
}

void SwitchModule::frontSwitchISR()
{
	SwitchModule::frontSwitchTriggered = true;
}

void SwitchModule::backButtonISR()
{
	SwitchModule::backButtonPressed = true;
}

void SwitchModule::bluetoothButtonISR()
{
	SwitchModule::bluetoothButtonPressed = true;
}

uint8_t SwitchModule::frontSwitchEventHandler()
{
	DEBUG.println("Handling front switch trigger!");
	disableFrontSwitch();
	if (digitalRead(PIN_FRONT_SWITCH) == LOW)
  {
    lock->openWithKey();
    SwitchModule::frontSwitchTriggered = false;
  }
  else
  {
    lock->closeWithKey();
    SwitchModule::frontSwitchTriggered = false;
  }
	enableFrontSwitch();
	return SUCCESS;
}

uint8_t SwitchModule::enableFrontSwitch()
{
	attachInterrupt(PIN_FRONT_SWITCH, SwitchModule::frontSwitchISR, ISR_DEFERRED | CHANGE);
	return SUCCESS;
}

uint8_t SwitchModule::disableFrontSwitch()
{
	detachInterrupt(PIN_FRONT_SWITCH);
	return SUCCESS;
}

uint8_t SwitchModule::enableBluetoothButton()
{
	attachInterrupt(PIN_BLUETOOTH_BUTTON, SwitchModule::bluetoothButtonISR, ISR_DEFERRED | FALLING);
	return SUCCESS;
}

uint8_t SwitchModule::disableBluetoothButton()
{
	detachInterrupt(PIN_BLUETOOTH_BUTTON);
	return SUCCESS;
}

uint8_t SwitchModule::bluetoothButtonHandler()
{
	DEBUG.println("Handling Bluetooth Button press!");
	disableBluetoothButton();
	blm->enableBonding();
	SwitchModule::bluetoothButtonPressed = false;
	enableBluetoothButton();
	return SUCCESS;
}

uint8_t SwitchModule::backButtonEventHandler()
{
	DEBUG.println("Handling Back Button press!");
	disableBackButton();
	lock->toggleState();
	SwitchModule::backButtonPressed = false;
	enableBackButton();
	return SUCCESS;
};

uint8_t SwitchModule::enableBackButton()
{
	attachInterrupt(PIN_BACK_BUTTON, SwitchModule::backButtonISR, ISR_DEFERRED | FALLING);
	return SUCCESS;
}

uint8_t SwitchModule::disableBackButton()
{
	detachInterrupt(PIN_BACK_BUTTON);
	return SUCCESS;
}
