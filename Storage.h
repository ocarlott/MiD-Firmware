#ifndef STORAGE_H
#define STORAGE_H

#include "Type.h"
#include "EEPROM.h"
#include "Constant.h"

class Storage
{
  private:
	int keyCodeAddr;
	int blueCodeAddr;
	int blueCode;
	int flag;
	struct KeyCodeManager kcm;

  public:
	Storage();
	int getBlueCode();
	struct KeyCode getPasscode(int index);
	void save();
	void setBlueCode(int blueCode);
	void addKeyCode(int keyCode);
	void removeKeyCode(int id);
	bool checkPasscode(int passcode);
	void clearStorage();
};

#endif