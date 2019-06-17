#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "KeypadModule.h"

class EventHandler
{
  public:
	static class KeypadModule *kpm;
	static void setup(class KeypadModule *k);
	static void keypadEventHandler(KeypadEvent key);
};

#endif