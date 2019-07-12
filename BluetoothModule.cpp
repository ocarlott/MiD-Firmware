#include "BluetoothModule.h"
#include "Debug.h"
#include "Lock.h"
#include "Storage.h"

uint8_t BluetoothModule::connectionCount;

volatile bool BluetoothModule::bondingEnabled;

uint8_t BluetoothModule::rssiIndex;

class Lock *BluetoothModule::LOCK;

class Storage *BluetoothModule::STORAGE;

class Debug *BluetoothModule::DEBUG;

int8_t BluetoothModule::rssi[RSSI_SAMPLE_COUNT];

BluetoothModule::BluetoothModule()
{
	this->txPower = SIGNAL_STRENGTH;
	this->name = SIGNAL_NAME;
	BluetoothModule::connectionCount = 0;
	this->bluefruit = &Bluefruit;
	this->bledis = new BLEDis();
	this->blebas = new BLEBas();
	this->bleAuth = new BLEService(0x9e124dde2782417388af53ff143faec1);
	this->pass = new BLECharacteristic(0x9e124dde2782417388af53ff143faec2);
	this->bleAlert = new BLEService(0x9e124dde2782417388af53ff143faee1);
	this->alertData = new BLECharacteristic(0x9e124dde2782417388af53ff143faee2);
	this->bleConfig = new BLEService(0x9e124dde2782417388af53ff143faef1);
	this->configData = new BLECharacteristic(0x9e124dde2782417388af53ff143faef2);
	BluetoothModule::rssiIndex = 0;
	for (uint8_t i = 0; i < RSSI_SAMPLE_COUNT; i++)
	{
		BluetoothModule::rssi[i] = CUT_OFF_THRESHOLD;
	}
	BluetoothModule::bondingEnabled = false;
};

uint8_t BluetoothModule::setup(class Debug *d, class Storage *st, class Lock *l)
{
	BluetoothModule::DEBUG = d;
	BluetoothModule::LOCK = l;
	BluetoothModule::STORAGE = st;
	this->bluefruit->autoConnLed(false);
	this->bluefruit->Advertising.addService(*this->bleAuth);
	this->bluefruit->begin(BLUETOOTH_MAX_CONNECTION, 0);
	this->bluefruit->setTxPower(this->txPower);
	this->bluefruit->setName(this->name);
	this->bluefruit->Periph.setConnectCallback(BluetoothModule::connectedCallback);
	this->bluefruit->Periph.setDisconnectCallback(BluetoothModule::disconnectedCallback);
	this->bluefruit->setRssiCallback(BluetoothModule::rssiChanged);
	bond_clear_all();
};

uint8_t BluetoothModule::configureServices()
{
	this->bledis->setManufacturer("MiD");
	this->bledis->setSoftwareRev("1.0.0");
	this->bledis->setModel("V1");
	this->bledis->begin();
	this->blebas->begin();
	this->blebas->write(100);
	this->bleAlert->begin();
	this->alertData->setProperties(CHR_PROPS_NOTIFY);
	this->alertData->setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
	// B0: b7-5 - Module, b4-0 - Status. B1: b7-4 - User, b3-0 - Action.
	this->alertData->setFixedLen(2);
	this->alertData->begin();
	this->bleAuth->begin();
	this->pass->setProperties(CHR_PROPS_WRITE_WO_RESP | CHR_PROPS_NOTIFY);
	this->pass->setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
	this->pass->setFixedLen(15);
	this->pass->setWriteCallback(BluetoothModule::handleAuth);
	this->pass->begin();
	this->bleConfig->begin();
	this->configData->setProperties(CHR_PROPS_WRITE_WO_RESP | CHR_PROPS_NOTIFY);
	this->configData->setPermission(SECMODE_NO_ACCESS, SECMODE_ENC_NO_MITM);
	// B0: b7-5 - Module, b4-0 - Action. B1-4: Value
	this->configData->setFixedLen(5);
	this->configData->setWriteCallback(BluetoothModule::handleConfig);
	this->configData->begin();
}

uint8_t BluetoothModule::enableBonding()
{
	BluetoothModule::bondingEnabled = true;
}

uint8_t BluetoothModule::disableBonding()
{
	BluetoothModule::bondingEnabled = false;
}

uint8_t BluetoothModule::startAdvertising()
{
	this->bluefruit->Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
	this->bluefruit->Advertising.addTxPower();
	this->bluefruit->Advertising.addName();
	this->bluefruit->Advertising.restartOnDisconnect(true);
	this->bluefruit->Advertising.setInterval(32, 244);
	this->bluefruit->Advertising.setFastTimeout(30);
	this->bluefruit->Advertising.start(0);
};

uint8_t BluetoothModule::stopAdvertising()
{
	this->bluefruit->Advertising.stop();
};

void BluetoothModule::connectedCallback(uint16_t conn_handle)
{
	// Get the reference to current connection
	BLEConnection *connection = Bluefruit.Connection(conn_handle);

	char central_name[32] = {0};
	connection->getPeerName(central_name, sizeof(central_name));
	connection->monitorRssi(10);

	BluetoothModule::DEBUG->println("Connected to: ");
	BluetoothModule::DEBUG->println(central_name);

	BluetoothModule::connectionCount++;
	BluetoothModule::DEBUG->print("Connection count: ", BluetoothModule::connectionCount);
	// Keep advertising if not reaching max
	if (BluetoothModule::connectionCount < MAX_PRPH_CONNECTION)
	{
		Bluefruit.Advertising.start(0);
	}
};

void BluetoothModule::disconnectedCallback(uint16_t conn_handle, uint8_t reason)
{
	(void)conn_handle;
	(void)reason;
	BluetoothModule::connectionCount--;
	Serial.println(BluetoothModule::connectionCount);
	BluetoothModule::LOCK->lock();
};

void BluetoothModule::handleAuth(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len)
{
	Serial.println(*data);
	if (BluetoothModule::bondingEnabled)
	{
		BLEConnection *connection = Bluefruit.Connection(conn_hdl);
		if (len == BLUETOOTH_PASSCODE_MAX_LENGTH)
		{
			if (BluetoothModule::STORAGE->checkBlueCode(data)) // 4D 69 44 00 00 00 00 00 00 00 00 00 00 00 00
			{
				if (connection->bond())
				{
					chr->write8(0);
				}
				else
				{
					chr->write8(1);
				}
				BluetoothModule::bondingEnabled = false;
			}
			else
			{
				Serial.println("Passcode is not correct!");
				chr->write8(2);
				connection->disconnect();
			}
		}
		else
		{
			Serial.print("Length is not enough: ");
			Serial.println(len);
		}
	}
}

void BluetoothModule::rssiChanged(uint16_t conn_hdl, int8_t rssi)
{
	BluetoothModule::rssi[BluetoothModule::rssiIndex++] = rssi;
	if (BluetoothModule::rssiIndex == RSSI_SAMPLE_COUNT)
	{
		BluetoothModule::rssiIndex = 0;
	}
	int8_t total = 0;
	for (uint8_t i = 0; i < RSSI_SAMPLE_COUNT; i++)
	{
		total += BluetoothModule::rssi[i];
	}
	total = total / RSSI_SAMPLE_COUNT;
	if (CUT_OFF_THRESHOLD - total > 5)
	{
		BLEConnection *connection = Bluefruit.Connection(conn_hdl);
		connection->stopRssi();
		BluetoothModule::LOCK->lock();
	}
}

void BluetoothModule::handleConfig(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len)
{
	Serial.println(*data);
}

uint8_t BluetoothModule::alert(Module m, Status s, uint8_t user, Action a)
{
	uint16_t data = m << 13 | s << 8 | user << 4 | a;
	this->alertData->notify16(data);
}
