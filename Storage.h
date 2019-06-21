#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include "Type.h"
#include "EEPROM.h"
#include "Constant.h"
#include "Debug.h"

class Storage
{
  private:
	uint8_t keyCodeAddr;
	uint8_t blueCodeAddr;
	uint32_t blueCode;
	uint8_t flag;
	uint8_t flagAddr;
	uint8_t fingerIdAddr;
	struct FingerprintManager fpm;
	struct KeyCodeManager kcm;

  public:
	Storage();
	uint8_t setup();
	uint8_t getBlueCode(uint32_t *returnValue);
	uint8_t getPasscode(uint8_t id, uint32_t *passcode);
	uint8_t save();
	uint8_t setBlueCode(uint32_t blueCode);
	uint8_t addKeyCode(uint32_t keyCode, uint8_t *returnId = NULL);
	uint8_t removeKeyCode(uint8_t id);
	bool checkPasscode(uint32_t passcode);
	uint8_t clearStorage();
	uint8_t getNextIdForFingerprint(uint8_t *id);
	uint8_t deleteFingerprintId(uint8_t id);
};

#endif