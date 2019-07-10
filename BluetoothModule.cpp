#include "BluetoothModule.h"

uint8_t BluetoothModule::connectionCount;

volatile bool BluetoothModule::bondingEnabled;

class Storage *BluetoothModule::storage;

class Lock *BluetoothModule::lock;

uint8_t BluetoothModule::rssiIndex;

int8_t BluetoothModule::rssi[10];

BluetoothModule::BluetoothModule()
{
	this->txPower = SIGNAL_STRENGTH;
	this->name = SIGNAL_NAME;
	BluetoothModule::connectionCount = 0;
	this->bluefruit = &Bluefruit;
	this->bledis = new BLEDis();
	this->blebas = new BLEBas();
	this->bleAuth = new BLEService(0x9e124dde2782417388af53ff143fec1);
	this->pass = new BLECharacteristic(0x9e124dde2782417388af53ff143fec2);
	this->bleAlert = new BLEService(0x9e124dde2782417388af53ff143fee1);
	this->alertData = new BLECharacteristic(0x9e124dde2782417388af53ff143fee2);
	this->bleConfig = new BLEService(0x9e124dde2782417388af53ff143fef1);
	this->configData = new BLECharacteristic(0x9e124dde2782417388af53ff143fef2);
  BluetoothModule::rssiIndex = 0;
  for (uint8_t i = 0; i < 10; i++)
  {
    BluetoothModule::rssi[i] = CUT_OFF_THRESHOLD;
  }
	BluetoothModule::bondingEnabled = false;
};

uint8_t BluetoothModule::setup(class Storage *s, class Lock *l)
{
	BluetoothModule::storage = s;
  BluetoothModule::lock = l;
	this->bluefruit->autoConnLed(false);
	this->bluefruit->begin(3, 0);
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
	this->bledis->setModel("V1");
	this->bledis->begin();
	this->blebas->begin();
	this->blebas->write(100);
	this->bleAlert->begin();
	this->alertData->setProperties(CHR_PROPS_NOTIFY);
	this->alertData->setPermission(SECMODE_SIGNED_WITH_MITM, SECMODE_NO_ACCESS);
	// B0: b7-5 - Module, b4-0 - Status. B1: b7-4 - User, b3-0 - Action.
	this->alertData->setFixedLen(2);
	this->alertData->begin();
	this->bleAuth->begin();
	this->pass->setProperties(CHR_PROPS_WRITE);
	this->pass->setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
	this->pass->setFixedLen(15);
	this->pass->setWriteCallback(BluetoothModule::handleAuth);
	this->pass->begin();
	this->bleConfig->begin();
	this->configData->setProperties(CHR_PROPS_WRITE);
	this->configData->setPermission(SECMODE_NO_ACCESS, SECMODE_SIGNED_WITH_MITM);
	// B0: b7-5 - Module, b4-0 - Action. B1: Value
	this->configData->setFixedLen(2);
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
  connection->monitorRssi(2);

	Serial.print("Connected to ");
	Serial.println(central_name);

	BluetoothModule::connectionCount++;
	Serial.print("Connection count: ");
	Serial.println(BluetoothModule::connectionCount);

	// Keep advertising if not reaching max
	if (BluetoothModule::connectionCount < MAX_PRPH_CONNECTION)
	{
		Serial.println("Keep advertising");
		Bluefruit.Advertising.start(0);
	}
};

void BluetoothModule::disconnectedCallback(uint16_t conn_handle, uint8_t reason)
{
	(void)conn_handle;
	(void)reason;

	Serial.println();
	Serial.println("Disconnected");

	BluetoothModule::connectionCount--;
	Serial.print("Connection count: ");
	Serial.println(BluetoothModule::connectionCount);
  Bluefruit.Advertising.stop();
  BluetoothModule::lock->lock();
};

void BluetoothModule::handleAuth(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len)
{
	Serial.println(*data);
	if (BluetoothModule::bondingEnabled)
	{
		BLEConnection *connection = Bluefruit.Connection(conn_hdl);
		if (len == BLUETOOTH_PASSCODE_MAX_LENGTH)
		{
			if (BluetoothModule::storage->checkBlueCode(data))
			{
				if (connection->requestPairing()) // 4D 69 44 00 00 00 00 00 00 00 00 00 00 00 00
				{
					Serial.println("Paired!");
				}
				else
				{
					Serial.println("Not Paired!");
				}
				BluetoothModule::bondingEnabled = false;
			}
		}
		else
		{
			Serial.println("Passcode is not correct!");
		}
	}
}

void BluetoothModule::rssiChanged(uint16_t conn_hdl, int8_t rssi)
{
  BluetoothModule::rssi[BluetoothModule::rssiIndex++] = rssi;
  if (BluetoothModule::rssiIndex == 10)
  {
    BluetoothModule::rssiIndex = 0;
  }
  int8_t total = 0;
  for (uint8_t i = 0; i < 10; i++)
  {
    total += BluetoothModule::rssi[i];
  }
  total = total / 10;
  if (CUT_OFF_THRESHOLD - total > 4)
  {
    BLEConnection *connection = Bluefruit.Connection(conn_hdl);
    connection->stopRssi();
    Serial.println("User went too far!");
    Bluefruit.Advertising.stop();
    BluetoothModule::lock->lock();
  }
}
