#include "AccelerometerModule.h"

uint8_t AccelerometerModule::timerForOrientationReading;

AccelerometerModule::AccelerometerModule(class Notification *n) : notifier(n)
{
	this->mma = Adafruit_MMA8451();
	this->threshold = 2000;
	this->deboundRate = 10;
	this->deboundCounter = 0;
	AccelerometerModule::timerForOrientationReading = 0;
}

uint8_t AccelerometerModule::setup()
{
	if (!this->mma.begin())
	{
		Debug::print("Acclerometer Sensor is not detected!");
		return FAILED;
	}
	else
	{
		Debug::print("Acclerometer Sensor detected!");
		this->mma.setRange(MMA8451_RANGE_4_G);
		this->mma.setDataRate(MMA8451_DATARATE_1_56_HZ);
		this->originalOrientation = this->mma.getOrientation();
		sensors_event_t newEvent;
		if (this->mma.getEvent(&newEvent))
		{
			Debug::print("Finished setting up accelerometer!");
			this->originalValue = int(sqrt(square(newEvent.acceleration.x * 1000) + square(newEvent.acceleration.y * 1000)));
		}
	}
	return SUCCESS;
}

uint8_t AccelerometerModule::read()
{
	this->deboundCounter += 1;
	if (this->deboundCounter % this->deboundRate == 0)
	{
		this->deboundCounter = 0;
		sensors_event_t newEvent;
		uint16_t newValue = 0;
		if (this->mma.getEvent(&newEvent))
		{
			AccelerometerModule::timerForOrientationReading += 1;
			newValue = uint16_t(sqrt(square(newEvent.acceleration.x * 1000) + square(newEvent.acceleration.y * 1000)));
			uint16_t dataRate = this->mma.getDataRate();
			Debug::print("New value for accelerometer: ", "");
			Debug::print(newValue);
			Debug::print("Data rate: ", "");
			Debug::print(dataRate);
			if (newValue - this->originalValue > this->threshold)
			{
				this->notifier->alertWarning("Force above threshold detected!");
			}
		}
		if (AccelerometerModule::timerForOrientationReading == COUNT_FOR_ORIENTATION_READING)
		{
			AccelerometerModule::timerForOrientationReading = 0;
			int newOrientation = this->mma.getOrientation();
			if (newOrientation != this->originalOrientation)
			{
				this->notifier->alertWarning("Change of lock's orientation detected!");
			}
		}
	}
	return SUCCESS;
}

uint8_t AccelerometerModule::getThreshold(uint16_t *th)
{
	*th = this->threshold;
	return SUCCESS;
}

uint8_t AccelerometerModule::setThreshold(uint16_t th)
{
	this->threshold = th;
	return SUCCESS;
}

uint8_t AccelerometerModule::calibrateOrientation()
{
	this->originalOrientation = this->mma.getOrientation();
	return SUCCESS;
}