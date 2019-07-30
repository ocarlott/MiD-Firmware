#define NRF32
#define ARDUINO_ARCH_NRF52
#include <Arduino.h>
#include "Debug.h"
#include "BluetoothModule.h"
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
BluetoothModule ble;
SwitchModule sm(&lock, &ble);

void callbackOnClosed(void)
{
	DEBUG.println("Close callback called!");
	ble.disableBonding();
	ble.stopAdvertising();
	kpm.enable();
	kpm.handleKey();
	fpm.enable();
	am.enable();
	am.clearInterrupt();
}

void callbackOnOpened(void)
{
	DEBUG.println("Open callback called!");
	ble.enableBonding();
	ble.startAdvertising();
	kpm.disable();
	fpm.disable();
	am.disable();
}

extern "C"
{
  void SysTick_Handler(void)
  {
    sm.checkFrontSwitch();
  }
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
	//	NRF_NFCT->TASKS_DISABLE = 1;
	//	NRF_PWM1->ENABLE = 0;
	//	NRF_PWM2->ENABLE = 0;
	//	NRF_SAADC->ENABLE = 0;
	uint8_t data[15] = {77, 105, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	ble.setup(&storage, &lock);
	ble.configureServices();
  SysTick_Config( (F_CPU/1000)*TICK_INTERVAL_MS );
	Serial.println("Done setup!");
}

void loop()
{
	DEBUG.flushDebugMessages();

	if (am.isReady())
	{
		am.handleInterrupt();
//		am.printNewData();
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
	//	sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
// Serial.println("End of loop!");
	__WFI();
	waitForEvent();
 sd_app_evt_wait();
}
