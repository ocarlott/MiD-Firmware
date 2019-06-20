#include "Notification.h"

void Notification::begin()
{
	pinMode(PIN_OUTPUT_SUCCESS, OUTPUT);
	pinMode(PIN_OUTPUT_FAILURE, OUTPUT);
}

void Notification::alertSuccess()
{
	Debug::print("Open!");
	digitalWrite(PIN_OUTPUT_SUCCESS, HIGH);
	delay(1000);
	digitalWrite(PIN_OUTPUT_SUCCESS, LOW);
}

void Notification::alertFailure()
{
	Debug::print("Failed!");
	digitalWrite(PIN_OUTPUT_FAILURE, HIGH);
	delay(1000);
	digitalWrite(PIN_OUTPUT_FAILURE, LOW);
}