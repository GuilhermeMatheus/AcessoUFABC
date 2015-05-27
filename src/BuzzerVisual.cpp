#include "BuzzerVisual.h"

BuzzerVisual::BuzzerVisual()
{ }

BuzzerVisual::BuzzerVisual(int8_t buzzerPin)
{
	this->_buzzerPin = buzzerPin;
}

void BuzzerVisual::Begin()
{
	pinMode(_buzzerPin, OUTPUT);
}

void BuzzerVisual::ShowSuccess(const char message[])
{
	digitalWrite(_buzzerPin, HIGH);
}

void BuzzerVisual::ShowWarning(const char message[])
{
	digitalWrite(_buzzerPin, HIGH);
}

void BuzzerVisual::ShowError(const char message[])
{
	digitalWrite(_buzzerPin, HIGH);
}

void BuzzerVisual::Clear()
{
	digitalWrite(_buzzerPin, LOW);
}