#include "AccessReg.h"

void AccessRegUtils::AccessReg_pack( const AccessReg &reg, byte buffer[] ) {
	for ( int i = 0; i < 4; i++ )
		buffer[i] = reg.mifareID[i];

	buffer[4] = ( reg.isMaster	<< 7 ) | ( reg.allowedDaysOfWeek );
	buffer[5] = ( reg.hourStart	<< 3 ) | ( reg.hourEnd	 >> 2 );
	buffer[6] = ( reg.hourEnd	<< 6 ) | ( reg.untilDay  << 1 ) | ( reg.untilMon >> 3 );
	buffer[7] = ( reg.untilMon	<< 5 ) | ( reg.untilYear >> 8 );
	buffer[8] = ( reg.untilYear	 % 256 );
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