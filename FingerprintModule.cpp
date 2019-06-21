#include "FingerprintModule.h"

// FingerprintModule::FingerprintModule()
// {
// 	pinMode(PIN_FINGERPRINT_RST, OUTPUT);
// 	digitalWrite(PIN_FINGERPRINT_RST, LOW);
// 	delay(100);
// 	digitalWrite(PIN_FINGERPRINT_RST, HIGH);
// 	pinMode(PIN_FINGERPRINT_WAKE, INPUT);
// 	Finger_SoftwareSerial_Init();
// 	Finger_Wait_Until_OK();
// }

// void FingerprintModule::poll()
// {
// 	Analysis_PC_Command();

// 	// If in sleep mode, turn on the auto wake-up function of the finger,
// 	//begin to check if the finger is pressed, and wake up the module and match it
// 	if (Finger_SleepFlag == 1)
// 	{
// 		Auto_Verify_Finger();
// 	}
// }
// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// comment these two lines if using hardware serial
//SoftwareSerial mySerial(2, 3);
//
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//// RX is white and should be connected to TX on board, TX is green
//void setup()
//{
//  Serial.begin(9600);
//  while (!Serial);  // For Yun/Leo/Micro/Zero/...
//  delay(100);
//  Debug::print("\n\nAdafruit finger detect test");
//
//  // set the data rate for the sensor serial port
//  this->reader.begin(57600);
//
//  if (this->reader.verifyPassword()) {
//    Debug::print("Found fingerprint sensor!");
//  } else {
//    Debug::print("Did not find fingerprint sensor :(");
//    while (1) { delay(1); }
//  }
//
//  this->reader.getTemplateCount();
//  Serial.print("Sensor contains "); Serial.print(this->reader.templateCount); Debug::print(" templates");
//  Debug::print("Waiting for valid this->reader...");
//}
//
//void loop()                     // run over and over again
//{
//  getFingerprintIDez();
//  delay(50);            //don't ned to run this at full speed.
//}
//
//uint8_t getFingerprintID() {
//  uint8_t p = this->reader.getImage();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Debug::print("Image taken");
//      break;
//    case FINGERPRINT_NOFINGER:
//      Debug::print("No finger detected");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Debug::print("Communication error");
//      return p;
//    case FINGERPRINT_IMAGEFAIL:
//      Debug::print("Imaging error");
//      return p;
//    default:
//      Debug::print("Unknown error");
//      return p;
//  }
//
//  // OK success!
//
//  p = this->reader.image2Tz();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Debug::print("Image converted");
//      break;
//    case FINGERPRINT_IMAGEMESS:
//      Debug::print("Image too messy");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Debug::print("Communication error");
//      return p;
//    case FINGERPRINT_FEATUREFAIL:
//      Debug::print("Could not find fingerprint features");
//      return p;
//    case FINGERPRINT_INVALIDIMAGE:
//      Debug::print("Could not find fingerprint features");
//      return p;
//    default:
//      Debug::print("Unknown error");
//      return p;
//  }
//
//  // OK converted!
//  p = this->reader.fingerFastSearch();
//  if (p == FINGERPRINT_OK) {
//    Debug::print("Found a print match!");
//  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Debug::print("Communication error");
//    return p;
//  } else if (p == FINGERPRINT_NOTFOUND) {
//    Debug::print("Did not find a match");
//    return p;
//  } else {
//    Debug::print("Unknown error");
//    return p;
//  }
//
//  // found a match!
//  Serial.print("Found ID #"); Serial.print(this->reader.fingerID);
//  Serial.print(" with confidence of "); Debug::print(this->reader.confidence);
//
//  return this->reader.fingerID;
//}
//
//// returns -1 if failed, otherwise returns ID #
//int getFingerprintIDez() {
//  uint8_t p = this->reader.getImage();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  p = this->reader.image2Tz();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  p = this->reader.fingerFastSearch();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  // found a match!
//  Serial.print("Found ID #"); Serial.print(this->reader.fingerID);
//  Serial.print(" with confidence of "); Debug::print(this->reader.confidence);
//  return this->reader.fingerID;
//}

// Optical Fingerprint section
FingerprintModule::FingerprintModule(class Storage *s, class Notification *n) : storage(s), notifier(n)
{
	this->ready = false;
	this->enrollmentRequested = false;
	this->waitTimeForCheckingFingerprint = 2000;
	SoftwareSerial conn(PIN_FINGERPRINT_GREEN, PIN_FINGERPRINT_WHITE);
	this->reader = &Adafruit_Fingerprint(&conn);
}

void FingerprintModule::isr()
{
	this->ready = true;
}

uint8_t FingerprintModule::setup()
{
	pinMode(PIN_FINGERPRINT_POWER, OUTPUT);
	pinMode(PIN_FINGERPRINT_WAKE, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE), this->isr, HIGH);
}

uint8_t FingerprintModule::run()
{
	uint8_t id;
	uint8_t status;
	if (this->enrollmentRequested)
	{
		status = this->enroll(&id);
	}
	else
	{
		status = this->check(&id);
	}
	if (status == SUCCESS)
	{
		// Do something with ID
		return SUCCESS;
	}
	return FAILED;
}

bool FingerprintModule::isReady()
{
	return this->ready;
}

uint8_t FingerprintModule::addFingerprint()
{
	this->enrollmentRequested = true;
	return SUCCESS;
}

uint8_t FingerprintModule::start()
{
	detachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE));
	this->reader->begin(57600);
	uint8_t counter = 5;
	bool available;
	do
	{
		available = this->reader->verifyPassword();
		if (available)
		{
			Debug::print("Found fingerprint sensor!");
		}
		else
		{
			Debug::print("Did not find fingerprint sensor :(");
		}
	} while (counter && !available);
	if (available)
	{
		digitalWrite(PIN_FINGERPRINT_POWER, HIGH);
		return SUCCESS;
	}
	return FAILED;
}

uint8_t FingerprintModule::stop()
{
	digitalWrite(PIN_FINGERPRINT_POWER, LOW);
	attachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE), this->isr, HIGH);
	return SUCCESS;
}

uint8_t FingerprintModule::getImage(unsigned long waitTime)
{
	unsigned long startTime = millis(), currentTime = 0;
	uint8_t debouncer = 0;
	uint8_t p = -1;
	do
	{
		p = this->reader->getImage();
		switch (p)
		{
		case FINGERPRINT_OK:
			Debug::print("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			if (debouncer % 10 == 0)
			{
				this->notifier->alertWarning("Communication error");
				debouncer = 1;
			}
			break;
		case FINGERPRINT_IMAGEFAIL:
			if (debouncer % 10 == 0)
			{
				this->notifier->alertWarning("Imaging error");
				debouncer = 1;
			}
			break;
		default:
			if (debouncer % 10 == 0)
			{
				this->notifier->alertWarning("Unknown error");
				debouncer = 1;
			}
			break;
		}
		debouncer++;
		currentTime = millis();
	} while (p != FINGERPRINT_OK && currentTime - startTime <= waitTime);
	if (p != FINGERPRINT_OK)
	{
		return FAILED;
	}
	return SUCCESS;
}

uint8_t FingerprintModule::convertImage()
{
	uint8_t p = this->reader->image2Tz(1);
	switch (p)
	{
	case FINGERPRINT_OK:
		Debug::print("Image converted");
		return SUCCESS;
	case FINGERPRINT_IMAGEMESS:
		this->notifier->alertWarning("Image too messy");
		return FAILED;
	case FINGERPRINT_PACKETRECIEVEERR:
		this->notifier->alertWarning("Communication error");
		return FAILED;
	case FINGERPRINT_FEATUREFAIL:
		this->notifier->alertWarning("Could not find fingerprint features");
		return FAILED;
	case FINGERPRINT_INVALIDIMAGE:
		this->notifier->alertWarning("Could not find fingerprint features");
		return FAILED;
	default:
		this->notifier->alertWarning("Unknown error");
		return FAILED;
	}
}

uint8_t FingerprintModule::waitForFingerRemoved()
{
	delay(2000);
	uint8_t p = 0;
	while (p != FINGERPRINT_NOFINGER)
	{
		p = this->reader->getImage();
	}
	return SUCCESS;
}

uint8_t FingerprintModule::createModel()
{
	uint8_t p = this->reader->createModel();
	if (p == FINGERPRINT_OK)
	{
		Debug::print("Prints matched! Model created!");
		return SUCCESS;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		this->notifier->alertWarning("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH)
	{
		this->notifier->alertWarning("Fingerprints did not match");
		return FAILED;
	}
	else
	{
		this->notifier->alertWarning("Unknown error");
		return FAILED;
	}
}

uint8_t FingerprintModule::storeModel(uint8_t id)
{
	uint8_t p = this->reader->storeModel(id);
	if (p == FINGERPRINT_OK)
	{
		Debug::print("Model stored!");
		return SUCCESS;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		this->notifier->alertWarning("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_BADLOCATION)
	{
		this->notifier->alertWarning("Could not store in that location");
		return FAILED;
	}
	else if (p == FINGERPRINT_FLASHERR)
	{
		this->notifier->alertWarning("Error writing to flash");
		return FAILED;
	}
	else
	{
		this->notifier->alertWarning("Unknown error");
		return FAILED;
	}
}

uint8_t FingerprintModule::enroll(uint8_t *returnId)
{
	if (!this->ready)
	{
		return FAILED;
	}
	if (SUCCESS != this->start())
	{
		return FAILED;
	}
	uint8_t id;
	if (SUCCESS == this->storage->getNextIdForFingerprint(&id))
	{
		Debug::print("Waiting for valid finger to enroll with ID #", "");
		Debug::print(id);
		if (SUCCESS != this->getImage(WAIT_TIME_FOR_FIRST_FINGERPRINT))
		{
			return FAILED;
		}
	}

	if (SUCCESS != this->convertImage())
	{
		return FAILED;
	}

	this->notifier->alertNextStep("Remove finger");
	// While (input from capacitive touch is not detected);
	this->waitForFingerRemoved();
	this->notifier->alertNextStep("Waiting for another image to confirm. Place same finger again!");
	if (SUCCESS != this->getImage(WAIT_TIME_FOR_SECOND_FINGERPRINT))
	{
		return FAILED;
	}

	// OK success!

	this->convertImage();

	// OK converted!

	if (SUCCESS != this->createModel())
	{
		return FAILED;
	}

	if (SUCCESS == this->storeModel(id))
	{
		*returnId = id;
		return SUCCESS;
	}
	return this->stop();
}

uint8_t FingerprintModule::check(uint8_t *returnId)
{
	if (SUCCESS != this->getImage(this->waitTimeForCheckingFingerprint))
	{
		return FAILED;
	}

	// OK success!

	if (SUCCESS != this->convertImage())
	{
		return FAILED;
	}

	// OK converted!
	uint8_t id;
	uint8_t status = this->search(&id);
	if (SUCCESS == status)
	{
		*returnId = id;
	}
	return status;
}

uint8_t FingerprintModule::search(uint8_t *returnId)
{
	uint8_t p = this->reader->fingerFastSearch();
	if (p == FINGERPRINT_OK)
	{
		Debug::print("Found a print match!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		Debug::print("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_NOTFOUND)
	{
		Debug::print("Did not find a match");
		return FAILED;
	}
	else
	{
		Debug::print("Unknown error");
		return FAILED;
	}

	// found a match!
	Debug::print("Found ID #");
	Debug::print(this->reader->fingerID, " ");
	Debug::print("with confidence of ");
	Debug::print(this->reader->confidence);
	*returnId = this->reader->fingerID;
	return SUCCESS;
}