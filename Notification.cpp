#include "Notification.h"

uint8_t Notification::setup()
{
	pinMode(PIN_OUTPUT_SUCCESS, OUTPUT);
	pinMode(PIN_OUTPUT_FAILURE, OUTPUT);
	return SUCCESS;
}

uint8_t Notification::alertSuccess()
{
	Debug::print("Open!");
	digitalWrite(PIN_OUTPUT_SUCCESS, HIGH);
	delay(1000);
	digitalWrite(PIN_OUTPUT_SUCCESS, LOW);
	return SUCCESS;
}

uint8_t Notification::alertFailure()
{
	Debug::print("Failed!");
	digitalWrite(PIN_OUTPUT_FAILURE, HIGH);
	delay(1000);
	digitalWrite(PIN_OUTPUT_FAILURE, LOW);
	return SUCCESS;
}

uint8_t Notification::alertWarning(char *msg)
{
	Debug::print(msg);
	return SUCCESS;
}

uint8_t Notification::alertNextStep(char *msg)
{
	Debug::print(msg);
	return SUCCESS;
}