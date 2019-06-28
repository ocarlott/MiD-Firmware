#include "KeypadModule.h"

byte rowPins[ROWS] = {PIN_KEYPAD_ROW_1, PIN_KEYPAD_ROW_2, PIN_KEYPAD_ROW_3, PIN_KEYPAD_ROW_4}; //connect to the row pinouts of the keypad (first 4 pins left to right looking at the pinpad)
byte colPins[COLS] = {PIN_KEYPAD_COL_1, PIN_KEYPAD_COL_2, PIN_KEYPAD_COL_3};				   //connect to the column pinouts of the keypad (last 3 pins left to right looking at the pinpad)
char keys[ROWS][COLS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};

Keypad_MC17 makeKeyPadObject()
{
	return Keypad_MC17(makeKeymap(keys), rowPins, colPins, ROWS, COLS, MCP_ADDR);
}

KeypadModule::KeypadModule(class Storage *s, class Lock *l) : keypad(makeKeyPadObject()), storage(s), lock(l)
{
	this->lastTime = millis();
	this->currentIndex = 0;
	this->enabled = true;
	this->startCapturing = false;
}

uint8_t KeypadModule::setup(void (*handler)(char))
{
	this->keypad.addEventListener(handler);
  DEBUG.println("Finished setting up Keypad Module.");
	return SUCCESS;
}

uint8_t KeypadModule::begin()
{
	this->keypad.begin();
	return SUCCESS;
}

uint8_t KeypadModule::enable()
{
	this->enabled = true;
	return SUCCESS;
}

uint8_t KeypadModule::disable()
{
	this->enabled = false;
	return SUCCESS;
}

uint8_t KeypadModule::handleKey(char key)
{
	if (this->enabled)
	{
		if (this->keypad.getState() == PRESSED)
		{
			DEBUG.print("Current key: ", key);
			unsigned long currentTime = millis();
			switch (key)
			{
			case '*':
				this->currentIndex = 0;
				this->startCapturing = true;
				break;
			case '#':
				if (this->startCapturing)
				{
					if (this->currentIndex == PASSCODE_LENGTH)
					{
						uint32_t passcode;
						this->getComputedPasscode(&passcode);
						this->currentIndex = 0;
						this->lock->openIfTrue(this->storage->checkPasscode(passcode));
					}
					this->startCapturing = false;
				}
				break;
			default:
				if (this->startCapturing)
				{
					if (this->currentIndex < PASSCODE_LENGTH)
					{
						if ((currentTime - this->lastTime <= MAX_DELAY_TIME_BETWEEN_KEYS))
						{
							this->passcode[this->currentIndex++] = key;
						}
						else
						{
							this->startCapturing = false;
						}
					}
					else
					{
						this->startCapturing = false;
					}
				}
				break;
			}
			this->lastTime = currentTime;
		}
	}
}

uint8_t KeypadModule::getComputedPasscode(uint32_t *passcode)
{
	uint32_t result = 0;
	for (uint8_t i = 0; i < PASSCODE_LENGTH; i++)
	{
		result = result * 10 + this->passcode[i] - 48;
	}
	*passcode = result;
	return SUCCESS;
}

uint8_t KeypadModule::getKey()
{
	this->keypad.getKey();
	return SUCCESS;
}
