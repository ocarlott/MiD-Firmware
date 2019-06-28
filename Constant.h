#ifndef CONSTANT_H
#define CONSTANT_H

#define PASSCODE_MAX_COUNT 10
#define PASSCODE_LENGTH 5
#define MAX_DELAY_TIME_BETWEEN_KEYS 2000
#define BLUETOOH_PASSCODE_LENGTH 5
#define ROWS 4
#define COLS 3
#define STARTED_VALUE 9876
#define MCP_ADDR 0x20
#define SERIAL_FREQ 9600
#define FINGERPRINT_MAX_COUNT 10
#define COUNT_FOR_ORIENTATION_READING 100 // Accelerometer read with freq of 1.56Hz so 100 times as slow will be ~ 64s
#define WAIT_TIME_FOR_FIRST_FINGERPRINT 1500
#define WAIT_TIME_FOR_SECOND_FINGERPRINT 10000

// Pins configuration
 #define PIN_KEYPAD_ROW_1 11 // Pin 4 on Keypad
 #define PIN_KEYPAD_ROW_2 12 // Pin 5 on Keypad
 #define PIN_KEYPAD_ROW_3 13 // Pin 6 on Keypad
 #define PIN_KEYPAD_ROW_4 14 // Pin 7 on Keypad
 #define PIN_KEYPAD_COL_1 8  // Pin 1 on Keypad
 #define PIN_KEYPAD_COL_2 9  // Pin 2 on Keypad
 #define PIN_KEYPAD_COL_3 10 // Pin 3 on Keypad
// #define PIN_FINGERPRINT_RST 9
// #define PIN_FINGERPRINT_WAKE 8 // These two are for capacitive
// These values are for arduino uno
#define PIN_FINGERPRINT_GREEN 10
#define PIN_FINGERPRINT_WHITE 11
#define PIN_FINGERPRINT_WAKE 2
#define PIN_ACCELEROMETER_INT 3
#define PIN_OUTPUT_SUCCESS 6
#define PIN_OUTPUT_FAILURE 7
#define PIN_MOTOR 5
#define PIN_BACK_BUTTON 8
#define PIN_FRONT_SWITCH 4
// These values are for adafruit
// #define PIN_FINGERPRINT_GREEN 2
// #define PIN_FINGERPRINT_WHITE 3
// #define PIN_FINGERPRINT_WAKE 4
// #define PIN_ACCELEROMETER_INT 5
// #define PIN_OUTPUT_SUCCESS 28
// #define PIN_OUTPUT_FAILURE 29
// #define PIN_MOTOR 27
// #define PIN_MCP_INT 30
// #define PIN_BACK_BUTTON
// #define PIN_FRONT_SWITCH

// Status code configuration
#define SUCCESS 0
#define FULL -1
#define INVALID_ID -2
#define FAILED -3
#define NO_MATCH -4

/*
* Accelerometer Module constants
*/
// Addresses
#define AM_DEFAULT_ADDRESS (0x1C) // if A is GND, its 0x1C
#define AM_REG_OUT_X_MSB 0x01
#define AM_REG_OUT_X_LSB 0x02
#define AM_REG_OUT_Y_MSB 0X03
#define AM_REG_OUT_Y_LSB 0X04
#define AM_REG_OUT_Z_MSB 0X05
#define AM_REG_OUT_Z_LSB 0X06
#define AM_REG_OFF_X 0X2F
#define AM_REG_OFF_Y 0X30
#define AM_REG_OFF_Z 0X31
#define AM_TRANSIENT_CFG 0X1D
#define AM_TRANSIENT_SCR 0X1E
#define AM_TRANSIENT_THS 0X1F
#define AM_TRANSIENT_COUNT 0X20
#define AM_PULSE_CFG 0X21
#define AM_PULSE_SRC 0X22
#define AM_PULSE_THSX 0X23
#define AM_PULSE_THSY 0X24
#define AM_PULSE_THSZ 0X25
#define AM_PULSE_TMLT 0X26
#define AM_PULSE_LTCY 0X27
#define AM_PULSE_WIND 0X28
#define AM_ASLP_COUNT 0X29
#define AM_REG_FF_MT_CFG 0x15
#define AM_REG_FF_MT_SRC 0x16
#define AM_REG_FF_MT_THS 0x17
#define AM_REG_FF_MT_COUNT 0x18
#define AM_INT_SOURCE 0x0C
#define AM_REG_SYSMOD 0x0B
#define AM_REG_WHOAMI 0x0D
#define AM_REG_XYZ_DATA_CFG 0x0E
#define AM_REG_PL_STATUS 0x10
#define AM_REG_PL_CFG 0x11
#define AM_REG_PL_COUNT 0x12
#define AM_REG_PL_THS 0x14
#define AM_REG_CTRL_REG1 0x2A // ODR = 800Hz, standby mode
#define AM_REG_CTRL_REG2 0x2B // Wake from sleep
#define AM_REG_CTRL_REG3 0x2C
#define AM_REG_CTRL_REG4 0x2D // Interrupt enable reg
#define AM_REG_CTRL_REG5 0x2E // Interrupt pin map
// Range
#define AM_RANGE_8_G 2 // +/- 8g
#define AM_RANGE_4_G 1 // +/- 4g (default value)
#define AM_RANGE_2_G 0 // +/- 2g
// Data Rate
#define AM_DATARATE_800_HZ 0  //  800Hz
#define AM_DATARATE_400_HZ 1  //  400Hz
#define AM_DATARATE_200_HZ 2  //  200Hz
#define AM_DATARATE_100_HZ 3  //  100Hz
#define AM_DATARATE_50_HZ 4   //   50Hz
#define AM_DATARATE_12_5_HZ 5 // 12.5Hz
#define AM_DATARATE_6_25HZ 6  // 6.25Hz
#define AM_DATARATE_1_56_HZ 7 // 1.56Hz
#define AM_DATARATE_MASK 7
// Portrait/Landscape constants
#define AM_PL_PUF 0
#define AM_PL_PUB 1
#define AM_PL_PDF 2
#define AM_PL_PDB 3
#define AM_PL_LRF 4
#define AM_PL_LRB 5
#define AM_PL_LLF 6
#define AM_PL_LLB 7
// Threshold scale
#define AM_THRESHOLD_STEP 63 // then divide by 1000 to get actual step of 0.063
#define AM_INPUT_SCALE 25	// 0 - 4g for tap detection (user will input value anywhere from 0 to 100)
#define AM_OUTPUT_SCALE 1000 // Instead of mutiplying with 9.809, we times output with 9809 and divide 100 at the receiver
#define AM_DEBOUNCE_MAX 1000000000
// Interrupt from AM
#define AM_ERROR_INT 0
#define AM_PL_INT 1
#define AM_MOTION_INT 2
#define AM_TAP_INT 3
#define AM_DATA_INT 4
#define AM_SLEEP_INT 5
// Interrupt bits
#define AM_MOTION_INT_BIT 0x04
#define AM_PL_INT_BIT 0x10
#define AM_TAP_INT_BIT 0x08
#define AM_SLEEP_INT_BIT 0x80
#define AM_DATA_READY_INT_BIT 0x01

#endif
