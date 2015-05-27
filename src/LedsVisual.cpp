#include "LedsVisual.h"

LedsVisual::LedsVisual()
{
}

LedsVisual::LedsVisual(int8_t greenLedPin, int8_t redLedPin)
{
	this->greenLedPin = greenLedPin;
	this->redLedPin = redLedPin;
}

void LedsVisual::Begin()
{
	pinMode(greenLedPin, OUTPUT);
	pinMode(redLedPin, OUTPUT);
}

void LedsVisual::ShowSuccess(const char message[])
{
	digitalWrite(greenLedPin, HIGH);
}

void LedsVisual::ShowWarning(const char message[])
{
	digitalWrite(greenLedPin, HIGH);
	digitalWrite(redLedPin, HIGH);
}

void LedsVisual::ShowError(const char message[])
{
	digitalWrite(redLedPin, HIGH);
}

void LedsVisual::Clear()
{
	digitalWrite(greenLedPin, LOW);
	digitalWrite(redLedPin, LOW);
}