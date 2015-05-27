#ifndef LEDSVISUAL_H
#define LEDSVISUAL_H

#include "IVisual.h"

#include "IVisual.h"
class LedsVisual :
	public IVisual
{
private:
	int8_t greenLedPin;
	int8_t redLedPin;
	LedsVisual();

public:
	LedsVisual(int8_t greenLedPin, int8_t redLedPin);

	void Begin();
	void Clear();

	void ShowSuccess(const char message[]);
	void ShowWarning(const char message[]);
	void ShowError(const char message[]);
};

#endif /* LEDSVISUAL_H */