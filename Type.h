#ifndef TYPE_H
#define TYPE_H

#include "Constant.h"

struct KeyCode
{
	int id;
	int code;
};

struct KeyCodeManager
{
	int numberOfKeyCodes;
	int id;
	struct KeyCode codes[PASSCODE_MAX_COUNT];
};

#endif