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

// Optical Fingerprint section
volatile bool FingerprintModule::ready;

FingerprintModule::FingerprintModule(class Storage *s, class Lock *l) : storage(s), lock(l)
{
	FingerprintModule::ready = false;
	this->enrollmentRequested = false;
	this->waitTimeForCheckingFingerprint = 2000;
	SoftwareSerial conn(PIN_FINGERPRINT_GREEN, PIN_FINGERPRINT_WHITE);
	this->reader = new Adafruit_Fingerprint(&conn);
}

void FingerprintModule::isr()
{
	FingerprintModule::ready = true;
}

uint8_t FingerprintModule::setup()
{
	pinMode(PIN_FINGERPRINT_WAKE, INPUT);
	this->reader->begin(57600);
	uint8_t counter = 5;
	bool available;
	do
	{
		available = this->reader->verifyPassword();
		if (available)
		{
			DEBUG.println("Found fingerprint sensor!");
		}
		else
		{
			DEBUG.println("Did not find fingerprint sensor :(");
		}
	} while (counter && !available);
	if (available)
	{
		this->reader->ledOff();
		attachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE), FingerprintModule::isr, HIGH);
		return SUCCESS;
	}
	return FAILED;
}

uint8_t FingerprintModule::run()
{
	uint8_t id;
	uint8_t status;
	this->reader->ledOn();
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
		NOTIFIER.alertSuccess();
		this->lock->openIfTrue(true);
		return SUCCESS;
	}
	else
	{
		NOTIFIER.alertFailure();
		DEBUG.println("Failed to open with fingerprint sensor!");
	}
	this->reader->ledOff();
	return FAILED;
}

bool FingerprintModule::isReady()
{
	return FingerprintModule::ready;
}

uint8_t FingerprintModule::addFingerprint()
{
	this->enrollmentRequested = true;
	return SUCCESS;
}

uint8_t FingerprintModule::start()
{
	detachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE));
	this->reader->ledOn();
	return FAILED;
}

uint8_t FingerprintModule::stop()
{
	this->reader->ledOff();
	attachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_WAKE), FingerprintModule::isr, HIGH);
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
			DEBUG.println("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			if (debouncer % 10 == 0)
			{
				NOTIFIER.alertWarning("Communication error");
				debouncer = 1;
			}
			break;
		case FINGERPRINT_IMAGEFAIL:
			if (debouncer % 10 == 0)
			{
				NOTIFIER.alertWarning("Imaging error");
				debouncer = 1;
			}
			break;
		default:
			if (debouncer % 10 == 0)
			{
				NOTIFIER.alertWarning("Unknown error");
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
		DEBUG.println("Image converted");
		return SUCCESS;
	case FINGERPRINT_IMAGEMESS:
		NOTIFIER.alertWarning("Image too messy");
		return FAILED;
	case FINGERPRINT_PACKETRECIEVEERR:
		NOTIFIER.alertWarning("Communication error");
		return FAILED;
	case FINGERPRINT_FEATUREFAIL:
		NOTIFIER.alertWarning("Could not find fingerprint features");
		return FAILED;
	case FINGERPRINT_INVALIDIMAGE:
		NOTIFIER.alertWarning("Could not find fingerprint features");
		return FAILED;
	default:
		NOTIFIER.alertWarning("Unknown error");
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
		DEBUG.println("Prints matched! Model created!");
		return SUCCESS;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		NOTIFIER.alertWarning("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH)
	{
		NOTIFIER.alertWarning("Fingerprints did not match");
		return FAILED;
	}
	else
	{
		NOTIFIER.alertWarning("Unknown error");
		return FAILED;
	}
}

uint8_t FingerprintModule::storeModel(uint8_t id)
{
	uint8_t p = this->reader->storeModel(id);
	if (p == FINGERPRINT_OK)
	{
		DEBUG.println("Model stored!");
		return SUCCESS;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		NOTIFIER.alertWarning("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_BADLOCATION)
	{
		NOTIFIER.alertWarning("Could not store in that location");
		return FAILED;
	}
	else if (p == FINGERPRINT_FLASHERR)
	{
		NOTIFIER.alertWarning("Error writing to flash");
		return FAILED;
	}
	else
	{
		NOTIFIER.alertWarning("Unknown error");
		return FAILED;
	}
}

uint8_t FingerprintModule::enroll(uint8_t *returnId)
{
	if (!FingerprintModule::ready)
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
		DEBUG.print("Waiting for valid finger to enroll with ID #", id);
		if (SUCCESS != this->getImage(WAIT_TIME_FOR_FIRST_FINGERPRINT))
		{
			return FAILED;
		}
	}

	if (SUCCESS != this->convertImage())
	{
		return FAILED;
	}

	NOTIFIER.alertNextStep("Remove finger");
	// While (input from capacitive touch is not detected);
	this->waitForFingerRemoved();
	NOTIFIER.alertNextStep("Waiting for another image to confirm. Place same finger again!");
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
		DEBUG.println("Found a print match!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		DEBUG.println("Communication error");
		return FAILED;
	}
	else if (p == FINGERPRINT_NOTFOUND)
	{
		DEBUG.println("Did not find a match");
		return FAILED;
	}
	else
	{
		DEBUG.println("Unknown error");
		return FAILED;
	}

	// found a match!
	DEBUG.print("Found ID #", this->reader->fingerID);
	DEBUG.print("Confidence score: ", this->reader->confidence);
	*returnId = this->reader->fingerID;
	return SUCCESS;
}

FingerprintModule::~FingerprintModule()
{
	delete this->reader;
}
