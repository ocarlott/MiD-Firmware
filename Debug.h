#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "Constant.h"

#define DEFAULT_DEBUG_VALUE 87

class Debug
{
  private:
	bool enabled = false;

  public:
	void enable()
	{
		enabled = true;
	};
	void disable()
	{
		enabled = false;
	};
	void print(const char *str, char c = DEFAULT_DEBUG_VALUE, const char *unit = "0")
	{
		if (enabled)
		{
			Serial.print(str);
			if (c == DEFAULT_DEBUG_VALUE)
			{
				Serial.println("");
			}
			else
			{
				if (unit[0] != '0')
				{
					Serial.print(c);
					Serial.println(unit);
				}
				else
				{
					Serial.println(c);
				}
			}
		}
	};
	void print(const char *str, uint8_t n = DEFAULT_DEBUG_VALUE, const char *unit = "")
	{
		if (enabled)
		{
			Serial.print(str);
			if (n == DEFAULT_DEBUG_VALUE)
			{
				Serial.println("");
			}
			else
			{
				if (unit[0] != '0')
				{
					Serial.print(n);
					Serial.println(unit);
				}
				else
				{
					Serial.println(n);
				}
			}
		}
	};
	void print(const char *str, uint16_t n = DEFAULT_DEBUG_VALUE, const char *unit = "")
	{
		if (enabled)
		{
			Serial.print(str);
			if (n == DEFAULT_DEBUG_VALUE)
			{
				Serial.println("");
			}
			else
			{
				if (unit[0] != '0')
				{
					Serial.print(n);
					Serial.println(unit);
				}
				else
				{
					Serial.println(n);
				}
			}
		}
	};
	void print(const char *str, uint32_t n = DEFAULT_DEBUG_VALUE, const char *unit = "")
	{
		if (enabled)
		{
			Serial.print(str);
			if (n == DEFAULT_DEBUG_VALUE)
			{
				Serial.println("");
			}
			else
			{
				if (unit[0] != '0')
				{
					Serial.print(n);
					Serial.println(unit);
				}
				else
				{
					Serial.println(n);
				}
			}
		}
	};
	void println(const char *str)
	{
		if (enabled)
		{
			Serial.println(str);
		}
	};
};

static Debug DEBUG;

#endif
