#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "Constant.h"

class Debug
{
  private:
	static bool _enable;

  public:
	static void enable();
	static void disable();
	static void print(char *str, char *ending = "\n");
	static void print(char c, char *ending = "\n");
	static void print(int n, char *ending = "\n");
	static void print(byte n, char *ending = "\n");
};

#endif