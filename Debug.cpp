#include "Debug.h"

extern HardwareSerial Serial;

Debug::Debug()
{
}

void Debug::captureSerialInputData()
{
	if (Serial.available())
	{
		Serial.write(Serial.read()); //send what has been received
		Serial.println();			 //print line feed character
	}
}