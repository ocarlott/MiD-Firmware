#include "SwitchModule.h"

volatile bool SwitchModule::frontSwitchTriggered;
volatile bool SwitchModule::backButtonPressed;

SwitchModule::SwitchModule(class Lock *l) : lock(l)
{
	SwitchModule::backButtonPressed = false;
	SwitchModule::frontSwitchTriggered = false;
}

uint8_t SwitchModule::setup()
{
	pinMode(PIN_BACK_BUTTON, INPUT_PULLUP);
	pinMode(PIN_FRONT_SWITCH, INPUT_PULLUP);
  enableFrontSwitch();
  enableBackButton();
	return SUCCESS;
}

bool SwitchModule::isReady()
{
  return SwitchModule::frontSwitchTriggered || SwitchModule::backButtonPressed;
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
}

void SwitchModule::frontSwitchISR()
{
	SwitchModule::frontSwitchTriggered = true;
}

void SwitchModule::backButtonISR()
{
	SwitchModule::backButtonPressed = true;
}

uint8_t SwitchModule::frontSwitchEventHandler()
{
  DEBUG.println("Handling front switch trigger!");
	disableFrontSwitch();
	lock->openWithKey();
	SwitchModule::frontSwitchTriggered = false;
	enableFrontSwitch();
	return SUCCESS;
}

uint8_t SwitchModule::enableFrontSwitch()
{
	attachInterrupt(PIN_FRONT_SWITCH, SwitchModule::frontSwitchISR, ISR_DEFERRED | FALLING);
	return SUCCESS;
}

uint8_t SwitchModule::disableFrontSwitch()
{
	detachInterrupt(PIN_FRONT_SWITCH);
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
