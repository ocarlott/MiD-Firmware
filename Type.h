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
	uint32_t bluetoothPasscode;
};

#endif
