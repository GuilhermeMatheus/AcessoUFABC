#ifndef KEYPADLISTENER_H
#define KEYPADLISTENER_H

#define ROW_1 3
#define ROW_2 2
#define ROW_3 1
#define ROW_4 0

#define THRESHOLD_TOLERANCE 30

#define MIN_THRESHOLD_ROW_1 128
#define MAX_THRESHOLD_ROW_1 384 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_2 384 + THRESHOLD_TOLERANCE
#define MAX_THRESHOLD_ROW_2 640 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_3 640 + THRESHOLD_TOLERANCE
#define MAX_THRESHOLD_ROW_3 896 - THRESHOLD_TOLERANCE

#define MIN_THRESHOLD_ROW_4 896 + THRESHOLD_TOLERANCE

#include "System.h"
#include "Arduino.h"

class KeyPadListener
{
private:
	int8_t pinColumnOne, pinColumnTwo, pinColumnThree, actualFunctionIndex;
	uint32_t cycle;
	
protected:
	char keyTable[3][4];
	int GetRowFromColumn(int8_t column);

public:
	KeyPadListener(int8_t pinColumnOne, int8_t pinColumnTwo, int8_t pinColumnThree);
	~KeyPadListener();

	int16_t GetState();
	char WaitForInput();
	uint8_t WaitForDigitInput();
	long GetPressTime(char c, long max);

};

#endif /* KEYPADLISTENER_H */