#ifndef TYPE_H
#define TYPE_H

#include "Constant.h"

struct KeyCodeManager
{
	uint8_t numberOfKeyCodes;
	uint32_t codes[PASSCODE_MAX_COUNT];
};

struct FingerprintManager
{
	uint8_t numberOfFingerprints;
	uint8_t ids[FINGERPRINT_MAX_COUNT];
};

struct CredentialsWrapper
{
	struct KeyCodeManager kcm;
	struct FingerprintManager fpm;
	uint8_t bluetoothPasscode[BLUETOOTH_PASSCODE_MAX_LENGTH];
};

enum Module
{
	Fingerprint_M = 1,
	Keypad_M = 2,
	Key_M = 3,
	BackButton_M = 4,
	Accelerometer_M = 5
};

enum Action
{
	Open_A = 1,
	Close_A = 2
};

enum Status
{
	Triggered = 1,
	Failed = 2,
	Error = 3
};

#endif
