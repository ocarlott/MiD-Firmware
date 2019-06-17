#ifndef CONSTANT_H
#define CONSTANT_H

const int PASSCODE_MAX_COUNT = 10;
const int PASSCODE_LENGTH = 5;
const int MAX_DELAY_TIME_BETWEEN_KEYS = 2000;
const int BLUETOOH_PASSCODE_LENGTH = 5;
const int ROWS = 4;
const int COLS = 3;
const int STARTED_VALUE = 9876;

enum Pin
{
	PIN_KEYPAD_ROW_1 = 12,
	PIN_KEYPAD_ROW_2 = 11,
	PIN_KEYPAD_ROW_3 = 10,
	PIN_KEYPAD_ROW_4 = 8,
	PIN_KEYPAD_COL_1 = 7,
	PIN_KEYPAD_COL_2 = 6,
	PIN_KEYPAD_COL_3 = 5,
	PIN_OUTPUT = 2,
	PIN_MOTOR = 9
};

#endif