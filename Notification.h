#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <Arduino.h>
#include "Constant.h"

class Notification
{
  private:
  public:
	Notification();
	void alertSuccess();
	void alertFailure();
};

#endif