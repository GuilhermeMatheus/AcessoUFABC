#include "AccessReg.h"

void AccessRegUtils::AccessReg_pack( const AccessReg &reg, byte buffer[] ) {
	for ( int i = 0; i < 4; i++ )
		buffer[i] = reg.mifareID[i];
	
	byte allowedDaysOfWeek = reg.allowedDaysOfWeek & (byte)0x7F;
	byte hourStart = reg.hourStart & (byte)0x1F;
	byte hourEnd = reg.hourEnd & (byte)0x1F;
	byte untilDay = reg.untilDay & (byte)0x1F;
	byte untilMon = reg.untilMon & (byte)0x0F;
	uint16_t untilYear = reg.untilYear & (uint16_t)0x1FFF;

	buffer[4] = (reg.isMaster << 7) | (allowedDaysOfWeek);
	buffer[5] = (hourStart << 3) | (hourEnd >> 2);
	buffer[6] = (hourEnd << 6) | (untilDay << 1) | (untilMon >> 3);
	buffer[7] = (untilMon << 5) | (untilYear >> 8);
	buffer[8] = (untilYear % 256);
}

void AccessRegUtils::AccessReg_unpack( const byte buffer[], AccessReg &reg ) {
	for ( int i = 0; i < 4; i++ )
		reg.mifareID[i] = buffer[i];

	reg.isMaster =			( buffer[4] >> 7 );
	reg.allowedDaysOfWeek = ( buffer[4]  % 128 );
	reg.hourStart =			( buffer[5] >> 3 );
	reg.hourEnd =			( ( buffer[5] % 8 ) << 2 )	| ( buffer[6] >> 6 );
	reg.untilDay =			( buffer[6] >> 1 ) % 32;
	reg.untilMon =			( ( buffer[6] % 2 ) << 3 )	| ( buffer[7] >> 5 );
	reg.untilYear =			( ( buffer[7] % 32 ) << 8 ) | buffer[8];
}