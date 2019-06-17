#include "Notification.h"

Notification::Notification()
{
	pinMode(PIN_OUTPUT, OUTPUT);
	Serial.begin(115200);
}

void Notification::alertSuccess()
{
	digitalWrite(PIN_OUTPUT, HIGH);
	Serial.println("Opened!");
	digitalWrite(PIN_OUTPUT, LOW);
}

void Notification::alertFailure()
{
	Serial.println("Failed!");
	digitalWrite(LED_BUILTIN, HIGH);
	delay(3000);
	digitalWrite(LED_BUILTIN, LOW);
}