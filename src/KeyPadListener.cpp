#include "KeyPadListener.h"

KeyPadListener::KeyPadListener(int8_t pinColumnOne, int8_t pinColumnTwo, int8_t pinColumnThree, int8_t maxFunctionsToCall)
{
	this->actualFunctionIndex = this->cycle = 0;

	this->pinColumnOne = pinColumnOne;
	this->pinColumnTwo = pinColumnTwo;
	this->pinColumnThree = pinColumnThree;
	this->maxFunctionsToCall = maxFunctionsToCall;

	//this->_function = (EventFunctor<KeyDownEventArgs>**)malloc(maxFunctionsToCall * sizeof(EventFunctor<KeyDownEventArgs>));

	this->keyTable[0][0] = '1'; this->keyTable[1][0] = '2'; this->keyTable[2][0] = '3';
	this->keyTable[0][1] = '4'; this->keyTable[1][1] = '5'; this->keyTable[2][1] = '6';
	this->keyTable[0][2] = '7'; this->keyTable[1][2] = '8'; this->keyTable[2][2] = '9';
	this->keyTable[0][3] = '*'; this->keyTable[1][3] = '0'; this->keyTable[2][3] = '#';
}

char KeyPadListener::WaitForInput()
{
	delay(300);

	int16_t result = -1;
	while ((result = GetState()) == -1)
		delay(300);

	System::BEEP();
	return (char)result;
}
int16_t KeyPadListener::GetState()
{
	int rowResult = -1;
	int16_t c = -1;

	if ((rowResult = GetRowFromColumn(pinColumnOne)) != -1)
		c = keyTable[0][rowResult];
	else if ((rowResult = GetRowFromColumn(pinColumnTwo)) != -1)
		c = keyTable[1][rowResult];
	else if ((rowResult = GetRowFromColumn(pinColumnThree)) != -1)
		c = keyTable[2][rowResult];

	Serial.print("\nGetState = ");
	Serial.print(c, DEC);

	return c;
}
uint8_t KeyPadListener::WaitForDigitInput()
{
	char c;

	do c = WaitForInput();
	while (c == '*' || c == '#');

	return (uint8_t)(c - '0');
}

long KeyPadListener::GetPressTime(char c, long max)
{
	long start = millis();
	long end = start;

	while (((char)GetState()) == c) {
		end = millis();
		delay(300);

		if (end - start >= max)
			return max;
	}

	return end - start;
}

void KeyPadListener::Listen()
{

	/*if (actualFunctionIndex == 0)
		return;

	cycle++;
	int rowResult = -1;
	char c = -1;

	if ((rowResult = GetRowFromColumn(pinColumnOne)) != -1)
		c = keyTable[0][rowResult];
	else if ((rowResult = GetRowFromColumn(pinColumnTwo)) != -1)
		c = keyTable[1][rowResult], 0;
	else if ((rowResult = GetRowFromColumn(pinColumnThree)) != -1)
		c = keyTable[2][rowResult], 0;
	
	if (rowResult != -1)
	{
		KeyDownEventArgs* e = new KeyDownEventArgs(c, cycle);

		for (int i = 0; i < actualFunctionIndex; i++)
			this->_function[i]->Call(e);

		delete(e);
	}*/
}

int KeyPadListener::GetRowFromColumn(int8_t column)
{
	int value = analogRead(column);

	if (value >= MIN_THRESHOLD_ROW_1 && value <= MAX_THRESHOLD_ROW_1)
		return ROW_1;
	if (value >= MIN_THRESHOLD_ROW_2 && value <= MAX_THRESHOLD_ROW_2)
		return ROW_2;
	if (value >= MIN_THRESHOLD_ROW_3 && value <= MAX_THRESHOLD_ROW_3)
		return ROW_3;
	if (value >= MIN_THRESHOLD_ROW_4)
		return ROW_4;

	return -1;
}

void KeyPadListener::AddKeyDownEventHandler(EventFunctor<KeyDownEventArgs>* function)
{
	/*if (actualFunctionIndex >= maxFunctionsToCall)
		return;

	this->_function[actualFunctionIndex++] = function;*/
}

KeyPadListener::~KeyPadListener()
{
}