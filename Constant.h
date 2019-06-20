#ifndef CONSTANT_H
#define CONSTANT_H

const int PASSCODE_MAX_COUNT = 10;
const int PASSCODE_LENGTH = 5;
const int MAX_DELAY_TIME_BETWEEN_KEYS = 2000;
const int BLUETOOH_PASSCODE_LENGTH = 5;
const int ROWS = 4;
const int COLS = 3;
const int STARTED_VALUE = 9876;
const int MCP_ADDR = 0x20;
const int SERIAL_FREQ = 9600;

enum Pin
{
	PIN_KEYPAD_ROW_1 = 11, // Pin 4 on Keypad
	PIN_KEYPAD_ROW_2 = 12, // Pin 5 on Keypad
	PIN_KEYPAD_ROW_3 = 13, // Pin 6 on Keypad
	PIN_KEYPAD_ROW_4 = 14, // Pin 7 on Keypad
	PIN_KEYPAD_COL_1 = 8,  // Pin 1 on Keypad
	PIN_KEYPAD_COL_2 = 9,  // Pin 2 on Keypad
	PIN_KEYPAD_COL_3 = 10, // Pin 3 on Keypad
	PIN_OUTPUT_SUCCESS = 2,
	PIN_OUTPUT_FAILURE = 3,
	PIN_MOTOR = 27,
	PIN_FINGERPRINT_RST = 9,
	PIN_FINGERPRINT_WAKE = 8
};

enum Status
{
	Success,
	Failure,
	Waiting,
	Error
};

#endif