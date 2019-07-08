#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include "Type.h"
#include "Constant.h"
#include "Debug.h"
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

extern Debug DEBUG;

class Storage
{
  private:
	struct CredentialsWrapper cw;

  public:
	uint8_t setup();
	uint8_t getBlueCode(uint8_t *returnValue);
	uint8_t getPasscode(uint8_t id, uint32_t *passcode);
	uint8_t save();
	uint8_t setBlueCode(uint8_t *blueCode);
	uint8_t addKeyCode(uint32_t keyCode, uint8_t *returnId = NULL);
	uint8_t removeKeyCode(uint8_t id);
	bool checkPasscode(uint32_t passcode);
	bool checkBlueCode(uint8_t *blueCode);
	uint8_t clearStorage();
	uint8_t getNextIdForFingerprint(uint8_t *id);
	uint8_t deleteFingerprintId(uint8_t id);
	uint8_t addFingerprintWithId(uint8_t id);
};

#endif
