#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

class Notification
{
  private:
  public:
	uint8_t setup();
	uint8_t alertSuccess();
	uint8_t alertFailure();
	uint8_t alertWarning(char *msg);
	uint8_t alertNextStep(char *msg);
};

#endif