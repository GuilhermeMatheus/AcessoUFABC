#include "IdleView.h"

IdleView::IdleView(LiquidCrystal_I2C* lcd, IDateTimeProvider* DateProvider)
	: ViewBase(lcd)
{
	this->DateProvider = DateProvider;
}

void IdleView::OnDraw()
{
	lcd->clear();
	
	String time = this->DateProvider->ToString();
	
	this->lcd->print(time);

	this->lcd->setCursor(0, 1);
	this->lcd->print(F("* por 3s p/ menu"));
}

bool IdleView::HasNewFrame()
{
	return GetElapsedTimeFromLastDraw() >= 60000;
}

IdleView::~IdleView()
{
}
