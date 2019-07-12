#define NRF32
#define ARDUINO_ARCH_NRF52

#include "AccelerometerModule.h"
#include "BluetoothModule.h"
#include "Debug.h"
#include "FingerprintModule.h"
#include "KeypadModule.h"
#include "Lock.h"
#include "MotorModule.h"
#include "Notification.h"
#include "Storage.h"
#include "SwitchModule.h"

class AccelerometerModule accelerometer;
class BluetoothModule ble;
class Debug debug;
class FingerprintModule fingerprint;
class KeypadModule keypad;
class Lock lock;
class MotorModule motor;
class Notification notifier;
class Storage storage;
class SwitchModule sw;

void callbackOnClosed(void)
{
	debug.println("Close callback called!");
	ble.disableBonding();
	ble.stopAdvertising();
	keypad.handleKey();
	fingerprint.enable();
	accelerometer.enable();
	accelerometer.clearInterrupt();
}

void callbackOnOpened(void)
{
	debug.println("Open callback called!");
	ble.enableBonding();
	ble.startAdvertising();
	fingerprint.disable();
	accelerometer.disable();
}

void setup()
{
	Serial.begin(SERIAL_FREQ);
	debug.enable();
	notifier.setup(&debug);
	storage.setup(&debug);
	accelerometer.setup(&debug, &notifier);
	fingerprint.setup(&debug, &storage, &lock, &notifier);
	keypad.setup(&debug, &storage, &lock);
	sw.setup(&debug, &ble, &lock);
	lock.addEventListener(callbackOnOpened, callbackOnClosed);
	//	NRF_NFCT->TASKS_DISABLE = 1;
	//	NRF_PWM1->ENABLE = 0;
	//	NRF_PWM2->ENABLE = 0;
	//	NRF_SAADC->ENABLE = 0;
	ble.setup(&debug, &storage, &lock);
	ble.configureServices();
	Serial.println("Done setup!");
}

void loop()
{
	debug.flushDebugMessages();

	if (accelerometer.isReady())
	{
		accelerometer.handleInterrupt();
		accelerometer.printNewData();
		accelerometer.clearInterrupt();
	};

	if (fingerprint.isReady())
	{
		fingerprint.run();
	};

	if (keypad.isReady())
	{
		keypad.handleKey();
	};

	if (sw.isReady())
	{
		sw.run();
	}
	//	sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
	__WFI();
	waitForEvent();
}
