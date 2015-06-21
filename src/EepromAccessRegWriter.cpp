#include "EepromAccessRegWriter.h"

EepromAccessRegWriter::EepromAccessRegWriter() { }

int EepromAccessRegWriter::Write( AccessReg value ) {
	byte emptyCode[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int eaddr = findSlot( emptyCode );
	
	if ( eaddr < 0 )
		return eaddr;

	byte buffer[ PACK_REG_SIZE ];
	AccessRegUtils::AccessReg_pack( value, buffer );

	for ( size_t i = 0; i < sizeof( buffer ); i++ )
		EEPROM.write( eaddr++, buffer[i] );

	return 1;
}

int EepromAccessRegWriter::Delete( byte code[4] ) {
	int eaddr = findSlot( code );

	if ( eaddr < 0 )
		return eaddr;

	for ( int i = 0; i < 4; i++ )
		EEPROM.write( eaddr++, 0xFF );

	return 1;
}

int EepromAccessRegWriter::findSlot( byte code[4] )
{
	for ( int i = OFFSET_ACS_Regs; i < MAX_EEPROM_SIZE; i += PACK_REG_SIZE ) {
		bool found = true;
		
		for ( int j = i; j < i + PACK_REG_SIZE; j++ ) {
			if ( EEPROM.read( i ) != code[j-i] ) {
				found = false;
				break;
			}
		}
		
		if ( found )
			return i;
	}

	return -1;
}

EepromAccessRegWriter::~EepromAccessRegWriter() { }