#include "FingerprintModule.h"

volatile bool FingerprintModule::ready;

FingerprintModule::FingerprintModule(class Storage *s, class Lock *l) : storage(s), lock(l)
{
	FingerprintModule::ready = false;
	this->enrollmentRequested = false;
	this->enabled = true;
	this->waitTimeForCheckingFingerprint = 2000;
}

void FingerprintModule::isr()
{
	FingerprintModule::ready = true;
}

uint8_t FingerprintModule::setup()
{
	pinMode(PIN_FINGERPRINT_YELLOW, INPUT);
	this->reader = new AdafruitFingerprint(&Serial1);
	this->reader->begin(57600);
	uint8_t counter = 3;
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
		counter--;
	} while (counter && !available);
	if (available)
	{
		this->reader->ledOff();
		this->enable();
		return SUCCESS;
	}
	return FAILED;
}

uint8_t FingerprintModule::enable()
{
	attachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_YELLOW), FingerprintModule::isr, ISR_DEFERRED | FALLING);
	this->enabled = true;
}

uint8_t FingerprintModule::disable()
{
	detachInterrupt(digitalPinToInterrupt(PIN_FINGERPRINT_YELLOW));
	this->enabled = false;
}

uint8_t FingerprintModule::run()
{
	if (this->enabled)
	{
		uint8_t id;
		uint8_t status;
		if (this->enrollmentRequested)
		{
			DEBUG.println("Start enrolling fingerprint!");
			status = this->enroll(&id);
			DEBUG.print("Enrollment completes with status: ", status);
			this->enrollmentRequested = false;
		}
		else
		{
			DEBUG.println("Start checking fingerprint!");
			status = this->check(&id);
			DEBUG.print("Checking fingerprint completes with status: ", status);
			FingerprintModule::ready = false;
			if (status == SUCCESS)
			{
				NOTIFIER.alertSuccess("Correct fingerprint!");
				this->lock->openIfTrue(true);
			}
			else
			{
				NOTIFIER.alertFailure("Failed to open with fingerprint sensor!");
			}
		}
		return status;
	}
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
	this->disable();
	this->reader->ledOn();
	return SUCCESS;
}

uint8_t FingerprintModule::stop()
{
	this->reader->ledOff();
	this->enable();
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
  this->start();
  uint8_t status = FAILED;
	if (FingerprintModule::ready)
	{
    uint8_t id;
    if (SUCCESS == this->storage->getNextIdForFingerprint(&id))
    {
      DEBUG.print("Waiting for valid finger to enroll with ID #", id);
      if (SUCCESS == this->getImage(WAIT_TIME_FOR_FIRST_FINGERPRINT))
      {
        if (SUCCESS == this->convertImage())
        {
          NOTIFIER.alertNextStep("Remove finger");
          // While (input from capacitive touch is not detected);
          this->waitForFingerRemoved();
          NOTIFIER.alertNextStep("Waiting for another image to confirm. Place same finger again!");
          if (SUCCESS == this->getImage(WAIT_TIME_FOR_SECOND_FINGERPRINT))
          {
            // OK success!
        
            this->convertImage();
          
            // OK converted!
          
            if (SUCCESS == this->createModel())
            {
              if (SUCCESS == this->storeModel(id))
              {
                *returnId = id;
                status = SUCCESS;
              }
            }
          }
        }
      }
    }
	}
  this->stop();
	return status;
}

uint8_t FingerprintModule::check(uint8_t *returnId)
{
  uint8_t status = FAILED;
  this->start();
	if (SUCCESS == this->getImage(this->waitTimeForCheckingFingerprint))
	{
		// OK success!

    if (SUCCESS == this->convertImage())
    {
      // OK converted!
      uint8_t id;
      status = this->search(&id);
      if (SUCCESS == status)
      {
        *returnId = id;
      }
    }
	}
  this->stop();
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
