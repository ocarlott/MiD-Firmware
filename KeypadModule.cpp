#include "KeypadModule.h"

volatile bool KeypadModule::ready;

KeypadModule::KeypadModule(class Storage *s, class Lock *l) : storage(s), lock(l)
{
	this->lastTime = millis();
	this->currentIndex = 0;
	this->enabled = true;
	this->addr = KP_ADDR;
	for (uint8_t i = 0; i < PASSCODE_LENGTH; i++)
	{
		this->passcode[i] = -1;
	}
	KeypadModule::ready = false;
  this->startEnteringPasscode = false;
}

uint8_t KeypadModule::setup()
{
	Wire.begin();
	writeRegister(KP_ELE_CFG, 0x00);
	writeRegister(KP_RES_CF, 0X63);
	delay(10);
	// Section A
	// This group controls filtering when data is > baseline.
	writeRegister(KP_MHD_R, 0x01);
	writeRegister(KP_NHD_R, 0x01);
	writeRegister(KP_NCL_R, 0x00);
	writeRegister(KP_FDL_R, 0x00);

	// Section B
	// This group controls filtering when data is < baseline.
	writeRegister(KP_MHD_F, 0x01);
	writeRegister(KP_NHD_F, 0x01);
	writeRegister(KP_NCL_F, 0xFF);
	writeRegister(KP_FDL_F, 0x02);

	// Section C
	// This group sets touch and release thresholds for each electrode
	writeRegister(KP_ELE0_T, KP_TOU_THRESH);
	writeRegister(KP_ELE0_R, KP_REL_THRESH);
	writeRegister(KP_ELE1_T, KP_TOU_THRESH);
	writeRegister(KP_ELE1_R, KP_REL_THRESH);
	writeRegister(KP_ELE2_T, KP_TOU_THRESH);
	writeRegister(KP_ELE2_R, KP_REL_THRESH);
	writeRegister(KP_ELE3_T, KP_TOU_THRESH);
	writeRegister(KP_ELE3_R, KP_REL_THRESH);
	writeRegister(KP_ELE4_T, KP_TOU_THRESH);
	writeRegister(KP_ELE4_R, KP_REL_THRESH);
	writeRegister(KP_ELE5_T, KP_TOU_THRESH);
	writeRegister(KP_ELE5_R, KP_REL_THRESH);
	writeRegister(KP_ELE6_T, KP_TOU_THRESH);
	writeRegister(KP_ELE6_R, KP_REL_THRESH);
	writeRegister(KP_ELE7_T, KP_TOU_THRESH);
	writeRegister(KP_ELE7_R, KP_REL_THRESH);
	writeRegister(KP_ELE8_T, KP_TOU_THRESH);
	writeRegister(KP_ELE8_R, KP_REL_THRESH);
	writeRegister(KP_ELE9_T, KP_TOU_THRESH);
	writeRegister(KP_ELE9_R, KP_REL_THRESH);
	writeRegister(KP_ELE10_T, KP_TOU_THRESH);
	writeRegister(KP_ELE10_R, KP_REL_THRESH);
	writeRegister(KP_ELE11_T, KP_TOU_THRESH);
	writeRegister(KP_ELE11_R, KP_REL_THRESH);

	// Section D
	// Set the Filter Configuration
	// Set ESI2
	writeRegister(KP_AFE1_CFG, 0x10);
	writeRegister(KP_AFE2_CFG, 0x04);

	// Section E
	// Electrode Configuration
	// Enable 6 Electrodes and set to run mode
	// Set KP_ELE_CFG to 0x00 to return to standby mode
	writeRegister(KP_ELE_CFG, 0x8C); // Enables all 12 Electrodes

	//	CL
	//	KP_ELEPROX
	//	KP_ELE:
	// Section F
	// Enable Auto Config and auto Reconfig
	/*writeRegister(ATO_CFG0, 0x0B);
	writeRegister(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   writeRegister(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	writeRegister(ATO_CFGT, 0xB5);*/
	// Target = 0.9*USL = 0xB5 @3.3V
	this->enable();
	DEBUG.println("Finished setting up Keypad Module.");
	return SUCCESS;
}

void KeypadModule::isr()
{
	KeypadModule::ready = true;
}

bool KeypadModule::isReady()
{
	return KeypadModule::ready;
}

uint8_t KeypadModule::enable()
{
	attachInterrupt(digitalPinToInterrupt(PIN_KEYPAD_INT), KeypadModule::isr, ISR_DEFERRED | FALLING);
	this->enabled = true;
	return SUCCESS;
}

uint8_t KeypadModule::disable()
{
	detachInterrupt(digitalPinToInterrupt(PIN_KEYPAD_INT));
	this->enabled = false;
	return SUCCESS;
}

uint8_t KeypadModule::handleKey()
{
    this->disable();
    uint8_t key;
    this->getKey(&key);
    if (key != NO_KEY)
    {
      DEBUG.print("Current key: ", key);
      unsigned long currentTime = millis();
      switch (key)
      {
      case KEY_STAR:
        if (!this->lock->isOpened())
        {
          this->currentIndex = 0;
        }
        this->startEnteringPasscode = true;
        break;
      case KEY_POUND:
        if (this->startEnteringPasscode)
        {
          if (this->lock->isOpened())
          {
            this->lock->lock();
          }
          else
          {
            uint32_t passcode;
            this->getComputedPasscode(&passcode);
            this->lock->openIfTrue(this->storage->checkPasscode(passcode));
            if (this->lock->isOpened())
            {
              this->startEnteringPasscode = false;
            }
          }
          this->currentIndex = 0;
        }
        else
        {
          this->lock->lock();
        }
        break;
      default:
        if (!this->lock->isOpened())
        {
          if ((currentTime - this->lastTime <= MAX_DELAY_TIME_BETWEEN_KEYS))
          {
            if (this->currentIndex < PASSCODE_LENGTH)
            {
              this->passcode[this->currentIndex++] = key;
            }
          }
          else
          {
            this->currentIndex = 0;
            this->passcode[this->currentIndex++] = key;
          }
        }
        break;
      }
      this->lastTime = currentTime;
    }
    DEBUG.println("Get here!");
    this->enable();
//	if (this->enabled)
//	{
//		this->disable();
//		uint8_t key;
//		this->getKey(&key);
//		if (key != NO_KEY)
//		{
//			DEBUG.print("Current key: ", key);
//			unsigned long currentTime = millis();
//			switch (key)
//			{
//			case KEY_STAR:
//				if (!this->lock->isOpened())
//        {
//          this->currentIndex = 0;
//        }
//				break;
//			case KEY_POUND:
//				if (this->lock->isOpened())
//				{
//					this->lock->lock();
//				}
//				else
//				{
//					uint32_t passcode;
//					this->getComputedPasscode(&passcode);
//					this->lock->openIfTrue(this->storage->checkPasscode(passcode));
//				}
//				this->currentIndex = 0;
//				break;
//			default:
//        if (!this->lock->isOpened())
//        {
//  				if ((currentTime - this->lastTime <= MAX_DELAY_TIME_BETWEEN_KEYS))
//  				{
//  					if (this->currentIndex < PASSCODE_LENGTH)
//  					{
//  						this->passcode[this->currentIndex++] = key;
//  					}
//  				}
//  				else
//  				{
//  					this->currentIndex = 0;
//  					this->passcode[this->currentIndex++] = key;
//  				}
//        }
//				break;
//			}
//			this->lastTime = currentTime;
//		}
//		this->enable();
//	}
	KeypadModule::ready = false;
	return SUCCESS;
}

uint8_t KeypadModule::getComputedPasscode(uint32_t *passcode)
{
	uint32_t result = 0;
	for (uint8_t i = 0; i < PASSCODE_LENGTH; i++)
	{
		result = result * 10 + this->passcode[i];
	}
	*passcode = result;
	return SUCCESS;
}

uint8_t KeypadModule::getKey(uint8_t *key)
{
	Wire.requestFrom((uint8_t)KP_ADDR, (uint8_t)2);
	uint8_t a, b;
	uint16_t status = 0;
	if (Wire.available() >= 2) // slave may send less than requested
	{
		a = Wire.read(); // receive a byte as character
		b = Wire.read();
		status = (a | (int)(b << 8));
	}
	*key = NO_KEY;
	if ((status & 0x0001) > 0)
		*key = KEY_ONE;
	if ((status & 0x0002) > 0)
		*key = KEY_FOUR;
	if ((status & 0x0004) > 0)
		*key = KEY_SEVEN;
	if ((status & 0x0008) > 0)
		*key = KEY_STAR;
	if ((status & 0x0010) > 0)
		*key = KEY_TWO;
	if ((status & 0x0020) > 0)
		*key = KEY_FIVE;
	if ((status & 0x0040) > 0)
		*key = KEY_EIGHT;
	if ((status & 0x0080) > 0)
		*key = KEY_ZERO;
	if ((status & 0x0100) > 0)
		*key = KEY_THREE;
	if ((status & 0x0200) > 0)
		*key = KEY_SIX;
	if ((status & 0x0400) > 0)
		*key = KEY_NINE;
	if ((status & 0x0800) > 0)
		*key = KEY_POUND;
	return SUCCESS;
}
