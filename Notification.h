#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

extern Debug DEBUG;

class Notification
{
  private:
  public:
	uint8_t setup()
	{
		pinMode(PIN_OUTPUT_SUCCESS, OUTPUT);
		pinMode(PIN_OUTPUT_FAILURE, OUTPUT);
		DEBUG.println("Finished setting up Notification");
		return SUCCESS;
	};

	uint8_t alertSuccess(char *msg)
	{
		DEBUG.println(msg);
		digitalWrite(PIN_OUTPUT_SUCCESS, HIGH);
		delay(1000);
		digitalWrite(PIN_OUTPUT_SUCCESS, LOW);
		return SUCCESS;
	};

	uint8_t alertFailure(char *msg)
	{
		DEBUG.println(msg);
		digitalWrite(PIN_OUTPUT_FAILURE, HIGH);
		delay(1000);
		digitalWrite(PIN_OUTPUT_FAILURE, LOW);
		return SUCCESS;
	};

	uint8_t alertWarning(const char *msg)
	{
		DEBUG.println(msg);
		return SUCCESS;
	};

	uint8_t alertNextStep(const char *msg)
	{
		DEBUG.println(msg);
		return SUCCESS;
	};
};

#endif
