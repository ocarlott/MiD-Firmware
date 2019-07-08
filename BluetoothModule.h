#ifndef BLUETOOTHMODULE_H
#define BLUETOOTHMODULE_H

#include "Constant.h"
#include "bluefruit.h"
#include "Storage.h"

class BluetoothModule
{
  private:
	int8_t txPower;
	char *name;
	static uint8_t connectionCount;
	static volatile bool bondingEnabled;
	static class Storage *storage;
	AdafruitBluefruit *bluefruit;
	BLEDis *bledis;
	BLEBas *blebas;
	BLEService *bleAuth;
	BLECharacteristic *pass;
	BLEService *bleAlert;
	BLECharacteristic *alertData;
	BLEService *bleConfig;
	BLECharacteristic *configData;

  public:
	BluetoothModule();
	uint8_t setup(class Storage *s);
	uint8_t configureServices();
	uint8_t startAdvertising();
	uint8_t stopAdvertising();
	uint8_t enableBonding();
	uint8_t disableBonding();
	static void connectedCallback(uint16_t conn_handle);
	static void disconnectedCallback(uint16_t conn_handle, uint8_t reason);
	static void handleAuth(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
};

#endif
