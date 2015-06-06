#ifndef VIEWBASE_H
#define VIEWBASE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <limits.h>

class ViewBase
{
private:
	ViewBase* LastView;
	long lastDraw;

protected:
	virtual void OnDraw() = 0;
	virtual bool HasNewFrame() = 0;

public:
	LiquidCrystal_I2C* lcd;

	ViewBase(LiquidCrystal_I2C* lcd) {
		this->lastDraw = -LONG_MAX;
		this->lcd = lcd;
	}

	void Draw() {
		if (!HasNewFrame() && LastView == this)
			return;

		OnDraw();

		LastView = this;
		lastDraw = millis();
	}

	long GetElapsedTimeFromLastDraw() {
		return millis() - lastDraw;
	}

	virtual ~ViewBase()
	{
	}
};

#endif /* VIEWBASE_H */