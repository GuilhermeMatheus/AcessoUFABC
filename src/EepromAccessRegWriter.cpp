#include "EepromAccessRegWriter.h"

EepromAccessRegWriter::EepromAccessRegWriter( IAccessRegWriter *fallback ) {
	this->fallback = fallback;
}

int EepromAccessRegWriter::Write( AccessReg value ) {
	_LOG( "on write" );
	
	byte emptyCode[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int eaddr = findSlot( emptyCode );
	
	if ( eaddr < 0 )
		return fallback->Write(value);

	byte buffer[ PACK_REG_SIZE ];
	AccessRegUtils::AccessReg_pack( value, buffer );

	for ( int i = 0; i < PACK_REG_SIZE; i++ )
		EEPROM.write( eaddr++, buffer[i] );

	return 1;
}

int EepromAccessRegWriter::Delete( const byte code[4] ) {
	_LOG( "on delete" );
	int eaddr = findSlot( code );

	if ( eaddr < 0 )
		return fallback->Delete( code );

	for ( int i = 0; i < 4; i++ )
		EEPROM.write( eaddr++, 0xFF );

	return 1;
}

int EepromAccessRegWriter::Get( const byte code[4], AccessReg &target ) {
	int addr = findSlot( code );

	if ( addr < 0 )
		return fallback->Get( code, target );

	byte buffer[ PACK_REG_SIZE ];
	for (int i = 0; i < PACK_REG_SIZE; i++)
		buffer[i] = EEPROM.read( i + addr );

	AccessRegUtils::AccessReg_unpack( buffer, target );
	return 1;
}

bool EepromAccessRegWriter::Clear() {
	//O acesso a mem�ria EEPROM dos microcontroladores avr atmega � custoso
	//em rela��o ao desempenho e seu ciclo de vida. Por este motivo, apenas
	//a regi�o destinada aos c�digos s�o limpos e n�o o registro inteiro.

	for ( int i = OFFSET_ACS_Regs; i < MAX_EEPROM_SIZE; i += PACK_REG_SIZE ) 
		for ( int j = i; j < i + 4; j++ )
			if ( EEPROM.read( j ) != 0xFF )
				EEPROM.write( j, 0xFF );

	return fallback->Clear();
}

int EepromAccessRegWriter::findSlot( const byte code[4] ) {
	for ( int i = OFFSET_ACS_Regs; i < MAX_EEPROM_SIZE; i += PACK_REG_SIZE ) {
		bool found = true;

		for ( int j = i; j < i + 4; j++ ) {
			if ( EEPROM.read( j ) != code[j-i] ) {
				found = false;
				break;
			}
		}
		
		if ( found ) {
			_LOGN( i, DEC );
			return i;
		}
	}

	_LOG( "findSlot: notFound" );
	return -1;
}

EepromAccessRegWriter::~EepromAccessRegWriter() { }