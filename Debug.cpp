#include "Debug.h"

bool Debug::enabled = false;

void Debug::enable()
{
	Debug::enabled = true;
};

void Debug::disable()
{
	Debug::enabled = false;
};

void Debug::print(char *str, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(str);
		Serial.print(ending);
	}
};

void Debug::print(char c, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(c);
		Serial.print(ending);
	}
};

void Debug::print(int n, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};

void Debug::print(uint8_t n, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};

void Debug::print(uint16_t n, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};

void Debug::print(uint32_t n, char *ending = "\n")
{
	if (Debug::enabled)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};