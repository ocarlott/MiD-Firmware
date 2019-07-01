#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

extern Debug DEBUG;

class Notification
{
  public:
	uint8_t setup()
	{
		pinMode(PIN_LED_RED, OUTPUT);
		pinMode(PIN_LED_GREEN, OUTPUT);
		pinMode(PIN_LED_BLUE, OUTPUT);
		DEBUG.println("Finished setting up Notification");
		return SUCCESS;
	};

	uint8_t alertSuccess(char *msg)
	{
		DEBUG.println(msg);
		digitalWrite(PIN_LED_GREEN, HIGH);
		delay(1000);
		digitalWrite(PIN_LED_GREEN, LOW);
		return SUCCESS;
	};

	uint8_t alertFailure(char *msg)
	{
		DEBUG.println(msg);
		digitalWrite(PIN_LED_RED, HIGH);
		delay(1000);
		digitalWrite(PIN_LED_RED, LOW);
		return SUCCESS;
	};

	uint8_t alertWarning(const char *msg)
	{
		digitalWrite(PIN_LED_RED, HIGH);
		digitalWrite(PIN_LED_GREEN, HIGH);
		delay(1000);
		digitalWrite(PIN_LED_GREEN, LOW);
		digitalWrite(PIN_LED_RED, LOW);
		DEBUG.println(msg);
		return SUCCESS;
	};

	uint8_t alertNextStep(const char *msg)
	{
		digitalWrite(PIN_LED_BLUE, HIGH);
		delay(1000);
		digitalWrite(PIN_LED_BLUE, LOW);
		DEBUG.println(msg);
		return SUCCESS;
	};
};

#endif
