#ifndef BLUETOOTHMODULE_H
#define BLUETOOTHMODULE_H

#include "Constant.h"
#include "bluefruit.h"
#include "Type.h"

class Debug;
class Lock;
class Storage;

class BluetoothModule
{
  private:
	int8_t txPower;
	char *name;
	static uint8_t connectionCount;
	static volatile bool bondingEnabled;
	AdafruitBluefruit *bluefruit;
	BLEDis *bledis;
	BLEBas *blebas;
	BLEService *bleAuth;
	BLECharacteristic *pass;
	BLEService *bleAlert;
	BLECharacteristic *alertData;
	BLEService *bleConfig;
	BLECharacteristic *configData;
	static int8_t rssi[RSSI_SAMPLE_COUNT];
	static uint8_t rssiIndex;
	static class Debug *DEBUG;
	static class Lock *LOCK;
	static class Storage *STORAGE;

  public:
	BluetoothModule();
	uint8_t setup(class Debug *d, class Storage *st, class Lock *l);
	uint8_t configureServices();
	uint8_t startAdvertising();
	uint8_t stopAdvertising();
	uint8_t enableBonding();
	uint8_t disableBonding();
	uint8_t alert(Module m, Status s, uint8_t user, Action a);
	static void connectedCallback(uint16_t conn_handle);
	static void disconnectedCallback(uint16_t conn_handle, uint8_t reason);
	static void handleAuth(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
	static void handleConfig(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
	static void rssiChanged(uint16_t conn_hdl, int8_t rssi);
};

#endif
