#include "KeypadModule.h"
#include "Constant.h"

byte rowPins[ROWS] = {PIN_KEYPAD_ROW_1, PIN_KEYPAD_ROW_2, PIN_KEYPAD_ROW_3, PIN_KEYPAD_ROW_4}; //connect to the row pinouts of the keypad (first 4 pins left to right looking at the pinpad)
byte colPins[COLS] = {PIN_KEYPAD_COL_1, PIN_KEYPAD_COL_2, PIN_KEYPAD_COL_3};				   //connect to the column pinouts of the keypad (last 3 pins left to right looking at the pinpad)
char keys[ROWS][COLS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};

Keypad makeKeyPadObject()
{
	return Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
}

KeypadModule::KeypadModule(class Storage *s, class Lock *l) : keypad(makeKeyPadObject()), storage(s), lock(l)
{
	this->lastTime = millis();
	this->currentIndex = 0;
	this->enabled = true;
}

void KeypadModule::setup(void (*handler)(char))
{
	this->keypad.addEventListener(handler);
}

void KeypadModule::enable()
{
	this->enabled = true;
}

void KeypadModule::disable()
{
	this->enabled = false;
}

void KeypadModule::handleKey(KeypadEvent key)
{
	if (this->enabled)
	{
		if (this->keypad.getState() == PRESSED)
		{
			int currentTime = millis();
			switch (key)
			{
			case '*':
				this->currentIndex = 0;
				break;
			case '#':
				if (this->currentIndex == PASSCODE_LENGTH)
				{
					int passcode = this->getComputedPasscode();
					this->currentIndex = 0;
					this->lock->openIfTrue(this->storage->checkPasscode(passcode));
				}
				break;
			default:
				if (currentTime - this->lastTime <= MAX_DELAY_TIME_BETWEEN_KEYS)
				{
					this->passcode[this->currentIndex++] = key;
				}
				else
				{
					this->currentIndex = 0;
					this->passcode[this->currentIndex++] = key;
				}
				break;
			}
			this->lastTime = currentTime;
		}
	}
}

int KeypadModule::getComputedPasscode()
{
	int result = 0;
	for (int i = 0; i < PASSCODE_LENGTH; i++)
	{
		result = result * 10 + this->passcode[i] - 48;
	}
	return result;
}

void KeypadModule::getKey()
{
	char key = this->keypad.getKey();
}