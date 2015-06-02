#include <Arduino.h>

struct AccessReg {
	byte					mifareID[4];
	byte					isMaster			: 1;
	byte					allowedDaysOfWeek	: 7;
	byte					hourStart			: 5;
	byte					hourEnd				: 5;
	byte					untilDay			: 5;
	byte					untilMon			: 4;
	byte					untilYAfter2k		: 8;
};