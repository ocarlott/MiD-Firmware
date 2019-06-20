#include "EventHandler.h"

class KeypadModule *EventHandler::kpm;

void EventHandler::setup(class KeypadModule *k)
{
	kpm = k;
}

void EventHandler::keypadEventHandler(KeypadEvent key)
{
	kpm->handleKey(key);
}