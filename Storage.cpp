#include "Storage.h"

Storage::Storage()
{
	EEPROM.get(0, this->flag);
	this->blueCodeAddr = sizeof(int);
	this->keyCodeAddr = sizeof(int) + sizeof(this->blueCode);
	if (this->flag == STARTED_VALUE)
	{
		EEPROM.get(this->blueCodeAddr, this->blueCode);
		EEPROM.get(this->keyCodeAddr, this->kcm);
	}
	else
	{
		this->kcm.numberOfKeyCodes = 0;
		this->kcm.id = 0;
		this->addKeyCode(12345);
		this->blueCode = 12345;
		for (int i = 1; i < PASSCODE_MAX_COUNT; i++)
		{
			this->kcm.codes[i] = {-1, -1};
		}
		this->flag = STARTED_VALUE;
	}
}

int Storage::getBlueCode()
{
	return this->blueCode;
}

struct KeyCode Storage::getPasscode(int index)
{
	if (index <= this->kcm.numberOfKeyCodes - 1 && index >= 0)
	{
		return this->kcm.codes[index];
	}
	return {-1, -1};
}

void Storage::save()
{
	EEPROM.put(this->blueCodeAddr, this->blueCode);
	EEPROM.put(this->keyCodeAddr, this->kcm);
	EEPROM.put(0, this->flag);
}

void Storage::setBlueCode(int blueCode)
{
	this->blueCode = blueCode;
}

void Storage::addKeyCode(int keyCode)
{
	struct KeyCode codeStruct;
	codeStruct.code = keyCode;
	codeStruct.id = this->kcm.id++;
	this->kcm.codes[this->kcm.numberOfKeyCodes++] = codeStruct;
}

void Storage::removeKeyCode(int id)
{
	for (int i = 0; i < this->kcm.numberOfKeyCodes; i++)
	{
		if (this->kcm.codes[i].id == id)
		{
			for (int j = i; j < this->kcm.numberOfKeyCodes - 1; j++)
			{
				this->kcm.codes[j] = this->kcm.codes[j + 1];
			}
			break;
		}
	}
}

bool Storage::checkPasscode(int passcode)
{
	int keyCount = this->kcm.numberOfKeyCodes;
	for (int i = 0; i < keyCount; i++)
	{
		if (this->kcm.codes[i].code == passcode)
		{
			return true;
		}
	}
	return false;
}

void Storage::clearStorage()
{
}