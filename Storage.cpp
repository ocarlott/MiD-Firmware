#include "Storage.h"

Storage::Storage()
{
	this->flagAddr = 0;
	this->blueCodeAddr = sizeof(this->flag) + this->flagAddr;
	this->keyCodeAddr = this->blueCodeAddr + sizeof(this->blueCode);
	this->fingerIdAddr = this->keyCodeAddr + sizeof(this->kcm);
}

uint8_t Storage::setup()
{
	Debug::print("Start flag is at address: ", "");
	Debug::print(this->flagAddr);
	Debug::print("Bluetooth passcode is at address: ", "");
	Debug::print(this->blueCodeAddr);
	Debug::print("Keypad passcodes are at address: ", "");
	Debug::print(this->keyCodeAddr);
	Debug::print("Fingerprint Ids are at address: ", "");
	Debug::print(this->fingerIdAddr);
	EEPROM.get(this->flagAddr, this->flag);
	if (this->flag == STARTED_VALUE)
	{
		EEPROM.get(this->blueCodeAddr, this->blueCode);
		EEPROM.get(this->keyCodeAddr, this->kcm);
		EEPROM.get(this->fingerIdAddr, this->fpm);
	}
	else
	{
		this->kcm.numberOfKeyCodes = 0;
		this->addKeyCode(12345);
		this->blueCode = 12345;
		this->fpm.numberOfFingerprints = 0;
		for (uint8_t i = 1; i < PASSCODE_MAX_COUNT; i++)
		{
			this->kcm.codes[i] = -1;
		}
		for (uint8_t i = i; i < FINGERPRINT_MAX_COUNT; i++)
		{
			this->fpm.ids[i] = -1;
		}
		this->flag = STARTED_VALUE;
		this->save();
	}
	return SUCCESS;
}

uint8_t Storage::getBlueCode(uint32_t *returnValue)
{
	*returnValue = this->blueCode;
	return SUCCESS;
}

uint8_t Storage::getPasscode(uint8_t id, uint32_t *passcode)
{
	if (id <= PASSCODE_MAX_COUNT && id > 0)
	{
		*passcode = this->kcm.codes[id - 1];
		return SUCCESS;
	}
	return INVALID_ID;
}

uint8_t Storage::save()
{
	EEPROM.put(this->flagAddr, this->flag);
	EEPROM.put(this->blueCodeAddr, this->blueCode);
	EEPROM.put(this->keyCodeAddr, this->kcm);
	EEPROM.put(this->fingerIdAddr, this->fpm);
	Debug::print("Saved data in Storage class!");
	return SUCCESS;
}

uint8_t Storage::setBlueCode(uint32_t blueCode)
{
	this->blueCode = blueCode;
	return SUCCESS;
}

uint8_t Storage::addKeyCode(uint32_t keyCode, uint8_t *returnId)
{
	for (uint8_t i = 0; i < PASSCODE_MAX_COUNT; i++)
	{
		if (this->kcm.codes[i] == -1)
		{
			this->kcm.codes[i] = keyCode;
			*returnId = i;
			return SUCCESS;
		}
	}
	return FULL;
}

uint8_t Storage::removeKeyCode(uint8_t id)
{
	if (id > 0 && id <= PASSCODE_MAX_COUNT)
	{
		this->kcm.codes[id - 1] = -1;
		return SUCCESS;
	}
	return INVALID_ID;
}

bool Storage::checkPasscode(uint32_t passcode)
{
	for (uint8_t i = 0; i < PASSCODE_MAX_COUNT; i++)
	{
		if (this->kcm.codes[i] == passcode)
		{
			return true;
		}
	}
	return false;
}

uint8_t Storage::clearStorage()
{
	this->flag = 0;
	EEPROM.put(this->flagAddr, this->flag);
	return SUCCESS;
}

uint8_t Storage::getNextIdForFingerprint(uint8_t *id)
{
	uint8_t count = this->fpm.numberOfFingerprints;
	if (count == 0)
	{
		*id = 1;
		return SUCCESS;
	}
	else
	{
		for (uint8_t i = 0; i < FINGERPRINT_MAX_COUNT; i++)
		{
			if (this->fpm.ids[i] == -1)
			{
				*id = i + 1;
				return SUCCESS;
			}
		}
		return FULL;
	}
}

uint8_t Storage::deleteFingerprintId(uint8_t id)
{
	if (id > 0 && id <= FINGERPRINT_MAX_COUNT)
	{
		this->fpm.ids[id] = -1;
		return SUCCESS;
	}
	return INVALID_ID;
}