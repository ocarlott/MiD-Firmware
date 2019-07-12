#include "Storage.h"
#include "Debug.h"

using namespace Adafruit_LittleFS_Namespace;

uint8_t Storage::setup(class Debug *d)
{
	DEBUG = d;
	File file(InternalFS);
	InternalFS.begin();
	file.open(FILE_DATA, FILE_O_READ);
	if (file)
	{
		DEBUG->println("Loading configuration data!");
		file.read(&this->cw, sizeof(this->cw));
		file.close();
	}
	else
	{
		file.close();
		this->cw.kcm.numberOfKeyCodes = 0;
		this->cw.fpm.numberOfFingerprints = 0;
		for (uint8_t i = 0; i < PASSCODE_MAX_COUNT; i++)
		{
			this->cw.kcm.codes[i] = 0;
		}
		this->addKeyCode(12345);
		for (uint8_t i = 0; i < FINGERPRINT_MAX_COUNT; i++)
		{
			this->cw.fpm.ids[i] = 0;
		}
		for (uint8_t i = 0; i < BLUETOOTH_PASSCODE_MAX_LENGTH; i++)
		{
			switch (i)
			{
			case 0:
				this->cw.bluetoothPasscode[i] = 'M';
				break;
			case 1:
				this->cw.bluetoothPasscode[i] = 'i';
				break;
			case 2:
				this->cw.bluetoothPasscode[i] = 'D';
				break;
			default:
				this->cw.bluetoothPasscode[i] = 0;
				break;
			}
		}
		DEBUG->println("Initialize configuration data!");
		this->save();
	}
	return SUCCESS;
}

uint8_t Storage::getBlueCode(uint8_t *returnValue)
{
	for (uint8_t i = 0; i < BLUETOOTH_PASSCODE_MAX_LENGTH; i++)
	{
		*returnValue = this->cw.bluetoothPasscode[i];
		returnValue++;
	}
	return SUCCESS;
}

uint8_t Storage::getPasscode(uint8_t id, uint32_t *passcode)
{
	if (id < PASSCODE_MAX_COUNT && id >= 0)
	{
		*passcode = this->cw.kcm.codes[id];
		return SUCCESS;
	}
	return INVALID_ID;
}

uint8_t Storage::save()
{
	this->clearStorage();
	File file(InternalFS);
	file.open(FILE_DATA, FILE_O_WRITE);
	if (file)
	{
		file.write((uint8_t *)(&this->cw), (uint16_t)sizeof(this->cw));
		file.close();
		DEBUG->println("Saved data in Storage class!");
	}
	else
	{
		DEBUG->println("Failed to create file to write!");
	}
	return SUCCESS;
}

uint8_t Storage::setBlueCode(uint8_t *blueCode)
{
	for (uint8_t i = 0; i < BLUETOOTH_PASSCODE_MAX_LENGTH; i++)
	{
		this->cw.bluetoothPasscode[i] = *blueCode;
		blueCode++;
	}
	this->save();
	return SUCCESS;
}

bool Storage::checkBlueCode(uint8_t *blueCode)
{
	for (uint8_t i = 0; i < BLUETOOTH_PASSCODE_MAX_LENGTH; i++)
	{
		Serial.printf("Char: %d\n", *blueCode);
		Serial.printf("Key: %d\n", this->cw.bluetoothPasscode[i]);
		if (this->cw.bluetoothPasscode[i] != *blueCode)
		{
			DEBUG->print("Failed at index: ", i);
			return false;
		}
		blueCode++;
	}
	return true;
}

uint8_t Storage::addKeyCode(uint32_t keyCode, uint8_t *returnId)
{
	for (uint8_t i = 0; i < PASSCODE_MAX_COUNT; i++)
	{
		if (this->cw.kcm.codes[i] == 0)
		{
			this->cw.kcm.codes[i] = keyCode;
			this->cw.kcm.numberOfKeyCodes++;
			*returnId = i;
			DEBUG->print("This passcode has been added: ", keyCode);
			this->save();
			return SUCCESS;
		}
	}
	return FULL;
}

uint8_t Storage::removeKeyCode(uint8_t id)
{
	if (id >= 0 && id < PASSCODE_MAX_COUNT)
	{
		this->cw.kcm.codes[id] = 0;
		this->save();
		return SUCCESS;
	}
	return INVALID_ID;
}

bool Storage::checkPasscode(uint32_t passcode)
{
	DEBUG->print("Passcode to check: ", passcode);
	for (uint8_t i = 0; i < PASSCODE_MAX_COUNT; i++)
	{
		DEBUG->print("Current passcode: ", this->cw.kcm.codes[i]);
		if (this->cw.kcm.codes[i] == passcode)
		{
			return true;
		}
	}
	return false;
}

uint8_t Storage::clearStorage()
{
	InternalFS.remove(FILE_DATA);
	return SUCCESS;
}

uint8_t Storage::getNextIdForFingerprint(uint8_t *id)
{
	if (this->cw.fpm.numberOfFingerprints == 0)
	{
		*id = 0;
		return SUCCESS;
	}
	else
	{
		for (uint8_t i = 0; i < FINGERPRINT_MAX_COUNT; i++)
		{
			if (this->cw.fpm.ids[i] == 0)
			{
				*id = i;
				return SUCCESS;
			}
		}
		return FULL;
	}
}

uint8_t Storage::deleteFingerprintId(uint8_t id)
{
	if (id >= 0 && id < FINGERPRINT_MAX_COUNT)
	{
		if (this->cw.fpm.ids[id] == 1)
		{
			this->cw.fpm.ids[id] = 0;
			this->save();
			return SUCCESS;
		}
	}
	return INVALID_ID;
}

uint8_t Storage::addFingerprintWithId(uint8_t id)
{
	if (id >= 0 && id < FINGERPRINT_MAX_COUNT)
	{
		if (this->cw.fpm.ids[id] == 0)
		{
			this->cw.fpm.ids[id] = 1;
			this->save();
			return SUCCESS;
		}
	}
	return INVALID_ID;
}
