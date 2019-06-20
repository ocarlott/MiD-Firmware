#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>
#include "Constant.h"
#include "Debug.h"

class Notification
{
  private:
  public:
	void begin();
	void alertSuccess();
	void alertFailure();
};

#endif