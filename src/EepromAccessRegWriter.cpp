#include "EepromAccessRegWriter.h"

EepromAccessRegWriter::EepromAccessRegWriter() { }

int EepromAccessRegWriter::Write( AccessReg value ) {
	int eaddr = findEmptySlot();
	
	if ( eaddr < 0 )
		return eaddr;

	const byte* p = ( const byte* )( const void* )&value;
	for ( size_t i = 0; i < sizeof( value ); i++ )
		EEPROM.write( eaddr++, *p++ );

	return 1;
}

int EepromAccessRegWriter::findEmptySlot()
{
	for (size_t i = OFFSET_ACS_Password; i < 1024; i += sizeof(AccessReg))
	{
		boolean found = true;
		for (size_t j = i; j < i + sizeof(AccessReg); j++)
			if (EEPROM.read(i) != 0xFF)
			{
				found = false;
				break;
			}

		if (found)
			return i;
	}

	return -1;
}

EepromAccessRegWriter::~EepromAccessRegWriter() { }