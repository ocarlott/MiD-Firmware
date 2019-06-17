#include "EventHandler.h"

void EventHandler::setup(class KeypadModule *k)
{
	EventHandler::kpm = k;
}

void EventHandler::keypadEventHandler(KeypadEvent key)
{
	EventHandler::kpm->handleKey(key);
}