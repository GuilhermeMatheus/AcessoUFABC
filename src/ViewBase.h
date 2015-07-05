#ifndef VIEWBASE_H
#define VIEWBASE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <limits.h>

#define UP_ARROW_IDX				0
#define DOWN_ARROW_IDX				1
#define RIGHT_ARROW_IDX				2
#define LEFT_ARROW_IDX				3


class ViewBase
{
private:
	ViewBase						*LastView;
	long							lastDraw;

protected:
	virtual void					OnDraw() = 0;
	virtual bool					HasNewFrame() = 0;

public:
	LiquidCrystal_I2C				*lcd;

	ViewBase( LiquidCrystal_I2C* lcd ) {
		this->lastDraw = -LONG_MAX;
		this->lcd = lcd;
	}

	void Draw() {
		if ( !HasNewFrame() && LastView == this )
			return;

		OnDraw();

		LastView = this;
		lastDraw = millis();
	}

	long GetElapsedTimeFromLastDraw() {
		return millis() - lastDraw;
	}

	static void InitDisplay( LiquidCrystal_I2C *lcd ){
		lcd->init();
		lcd->backlight();

		byte Down_ArrowDef[8] = { B00000, B00000, B00000, B11111, B01110, B00100, B00000, B00000 };
		byte Up_ArrowDef[8] = { B00000, B00000, B00100, B01110, B11111, B00000, B00000, B00000 };
		byte Right_ArrowDef[8] = { B00000, B01000, B01100, B01110, B01110, B01100, B01000, B00000 };
		byte Left_ArrowDef[8] = { B00000, B00010, B00110, B01110, B01110, B00110, B00010, B00000 };

		lcd->createChar( UP_ARROW_IDX, Up_ArrowDef );
		lcd->createChar( DOWN_ARROW_IDX, Down_ArrowDef );
		lcd->createChar( LEFT_ARROW_IDX, Left_ArrowDef );
		lcd->createChar( RIGHT_ARROW_IDX, Right_ArrowDef );

		lcd->clear();
	}

	virtual ~ViewBase() { }
};

#endif /* VIEWBASE_H */