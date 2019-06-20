#include "Debug.h"

bool Debug::_enable = false;

void Debug::enable()
{
	Debug::_enable = true;
};

void Debug::disable()
{
	Debug::_enable = false;
};

void Debug::print(char *str, char *ending = "\n")
{
	if (Debug::_enable)
	{
		Serial.print(str);
		Serial.print(ending);
	}
};

void Debug::print(char c, char *ending = "\n")
{
	if (Debug::_enable)
	{
		Serial.print(c);
		Serial.print(ending);
	}
};
void Debug::print(int n, char *ending = "\n")
{
	if (Debug::_enable)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};
void Debug::print(byte n, char *ending = "\n")
{
	if (Debug::_enable)
	{
		Serial.print(n);
		Serial.print(ending);
	}
};