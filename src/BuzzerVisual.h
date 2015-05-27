#ifndef BUZZERVISUAL_H
#define BUZZERVISUAL_H

#include "IVisual.h"
class BuzzerVisual :
	public IVisual
{
private:
	int8_t _buzzerPin;
	BuzzerVisual();

public:
	BuzzerVisual(int8_t buzzerPin);

	void Begin();
	void Clear();

	void ShowSuccess(const char message[]);
	void ShowWarning(const char message[]);
	void ShowError(const char message[]);
};

#endif /* BUZZERVISUAL_H */