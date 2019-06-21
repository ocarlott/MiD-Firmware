#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "Constant.h"

class Debug
{
  private:
	static bool enabled;

  public:
	static void enable();
	static void disable();
	static void print(char *str, char *ending = "\n");
	static void print(char c, char *ending = "\n");
	static void print(int n, char *ending = "\n");
	static void print(uint8_t n, char *ending = "\n");
	static void print(uint16_t n, char *ending = "\n");
	static void print(uint32_t n, char *ending = "\n");
};

#endif