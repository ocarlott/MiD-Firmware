#ifndef CONSTANT_H
#define CONSTANT_H

#define PASSCODE_MAX_COUNT 10
#define PASSCODE_LENGTH 5
#define MAX_DELAY_TIME_BETWEEN_KEYS 2000
#define BLUETOOTH_PASSCODE_MAX_LENGTH 15
#define MAX_PRPH_CONNECTION 5
#define STARTED_VALUE 9876
#define MCP_ADDR 0x20
#define SERIAL_FREQ 115200
#define FINGERPRINT_MAX_COUNT 10
#define COUNT_FOR_ORIENTATION_READING 100 // Accelerometer reads with freq of 100Hz in active mode and 6.25Hz in standby mode
#define WAIT_TIME_FOR_FIRST_FINGERPRINT 1500
#define WAIT_TIME_FOR_SECOND_FINGERPRINT 10000
#define SIGNAL_STRENGTH -40
#define CUT_OFF_THRESHOLD -65
#define SIGNAL_NAME "MiD-0000001"
#define FILE_DATA "/data"

// Pins configuration
#define PIN_FINGERPRINT_YELLOW A5
#define PIN_ACCELEROMETER_INT A1
#define PIN_KEYPAD_INT A0
#define PIN_LED_RED 5
#define PIN_LED_GREEN 6
#define PIN_LED_BLUE 9
#define PIN_MOTOR A2
#define PIN_BACK_BUTTON A4
#define PIN_FRONT_SWITCH A3
#define PIN_BLUETOOTH_BUTTON 13

// Status code configuration
#define SUCCESS 0
#define FULL 1
#define INVALID_ID 2
#define FAILED 3
#define NO_MATCH 4

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
#define AM_KNOCK_INT 3
#define AM_DATA_INT 4
#define AM_SLEEP_INT 5
// Interrupt bits
#define AM_MOTION_INT_BIT 0x04
#define AM_PL_INT_BIT 0x10
#define AM_KNOCK_INT_BIT 0x08
#define AM_SLEEP_INT_BIT 0x80
#define AM_DATA_READY_INT_BIT 0x01

// Keypad constants
#define KP_ADDR 0x5B

// Register Defines
#define KP_E0BV 0x1E
#define KP_E1BV 0x1F
#define KP_E2BV 0x20
#define KP_E3BV 0x21
#define KP_E4BV 0x22
#define KP_E5BV 0x23
#define KP_E6BV 0x24
#define KP_E7BV 0x25
#define KP_E8BV 0x26
#define KP_E9BV 0x27
#define KP_E10BV 0x28
#define KP_E11BV 0x29
#define KP_E12BV 0x2A

#define KP_MHD_R 0x2B
#define KP_NHD_R 0x2C
#define KP_NCL_R 0x2D
#define KP_FDL_R 0x2E
#define KP_MHD_F 0x2F
#define KP_NHD_F 0x30
#define KP_NCL_F 0x31
#define KP_FDL_F 0x32

#define KP_ELE0_T 0x41
#define KP_ELE0_R 0x42
#define KP_ELE1_T 0x43
#define KP_ELE1_R 0x44
#define KP_ELE2_T 0x45
#define KP_ELE2_R 0x46
#define KP_ELE3_T 0x47
#define KP_ELE3_R 0x48
#define KP_ELE4_T 0x49
#define KP_ELE4_R 0x4A
#define KP_ELE5_T 0x4B
#define KP_ELE5_R 0x4C
#define KP_ELE6_T 0x4D
#define KP_ELE6_R 0x4E
#define KP_ELE7_T 0x4F
#define KP_ELE7_R 0x50
#define KP_ELE8_T 0x51
#define KP_ELE8_R 0x52
#define KP_ELE9_T 0x53
#define KP_ELE9_R 0x54
#define KP_ELE10_T 0x55
#define KP_ELE10_R 0x56
#define KP_ELE11_T 0x57
#define KP_ELE11_R 0x58

#define KP_AFE1_CFG 0x5C
#define KP_AFE2_CFG 0x5D
#define KP_ELE_CFG 0x5E

#define KP_GPIO_CTRL0 0x73
#define KP_GPIO_CTRL1 0x74
#define KP_GPIO_DATA 0x75
#define KP_GPIO_DIR 0x76
#define KP_GPIO_EN 0x77
#define KP_GPIO_SET 0x78
#define KP_GPIO_CLEAR 0x79
#define KP_GPIO_TOGGLE 0x7A

#define KP_ATO_CFG0 0x7B
#define KP_ATO_CFGU 0x7D
#define KP_ATO_CFGL 0x7E
#define KP_ATO_CFGT 0x7F
#define KP_RES_CF 0x80

#define KP_TOU_THRESH 0x06
#define KP_REL_THRESH 0x03
#define KP_START_BV 0x90

#define NO_KEY 15
#define KEY_ONE 1
#define KEY_TWO 2
#define KEY_THREE 3
#define KEY_FOUR 4
#define KEY_FIVE 5
#define KEY_SIX 6
#define KEY_SEVEN 7
#define KEY_EIGHT 8
#define KEY_NINE 9
#define KEY_ZERO 0
#define KEY_STAR 11
#define KEY_POUND 12

#endif
