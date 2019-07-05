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
  NRF_NFCT->TASKS_DISABLE = 1;
  NRF_PWM1->ENABLE = 0;
  NRF_PWM2->ENABLE = 0;
  NRF_SAADC->ENABLE = 0;
	Serial.println("Done setup!");
}

void loop()
{
	DEBUG.flushDebugMessages();
 
	if (am.isReady())
  {
    am.handleInterrupt();
    am.printNewData();
    am.clearInterrupt();
  };

  if (fpm.isReady())
  {
    fpm.run();
  };
 
  if (kpm.isReady())
  {
    kpm.handleKey();
  };
  
	if (sm.isReady())
	{
		sm.run();
	}

  sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
  __WFI();
  waitForEvent();
}
