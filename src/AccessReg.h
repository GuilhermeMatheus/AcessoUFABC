#ifndef ACCESSREG_H
#define ACCESSREG_H

#include <Arduino.h>

struct AccessReg {
	byte						mifareID[4];
	byte						isMaster			: 1;
	byte						allowedDaysOfWeek	: 7;
	byte						hourStart			: 5;
	byte						hourEnd				: 5;
	byte						untilDay			: 5;
	byte						untilMon			: 4;
	uint16_t					untilYear			: 13;
};

class AccessRegUtils {

public:
	/*
		The buffer array MUST HAVE at least 9 bytes!
	*/
	static void					AccessReg_pack( const AccessReg &reg, byte buffer[] );
	static void					AccessReg_unpack( const byte buffer[], AccessReg &reg );
};


#endif /* ACCESSREG_H */