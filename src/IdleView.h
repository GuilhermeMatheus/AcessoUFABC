#ifndef IDLEVIEW_H
#define IDLEVIEW_H

#include <Arduino.h>
#include "ViewBase.h"
#include "IDateTimeProvider.h"

class IdleView :
	public ViewBase
{
private:
	IDateTimeProvider* DateProvider;

protected:
	void OnDraw();
	bool HasNewFrame();

public:

	IdleView(LiquidCrystal_I2C* lcd, IDateTimeProvider* DateProvider);

	~IdleView();
};

#endif /* IDLEVIEW_H */