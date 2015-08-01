#include "AT24C256RegWriter.h"

AT24C256RegWriter::AT24C256RegWriter( int deviceAddress ) {
	this->deviceAddress = deviceAddress;
}

void AT24C256RegWriter::i2c_eeprom_write_byte( unsigned int eeaddress, byte data ) {
	int rdata = data;
	Wire.beginTransmission( deviceAddress );
	Wire.write((int)(eeaddress >> 8)); // MSB
	Wire.write((int)(eeaddress & 0xFF)); // LSB
	Wire.write(rdata);
	Wire.endTransmission();
}

void AT24C256RegWriter::i2c_eeprom_write_page(unsigned int eeaddresspage, byte* data, byte length) {
	Wire.beginTransmission( deviceAddress );
	Wire.write((int)(eeaddresspage >> 8)); // MSB
	Wire.write((int)(eeaddresspage & 0xFF)); // LSB
	byte c;
	for (c = 0; c < length; c++)
		Wire.write(data[c]);
	Wire.endTransmission();
}

uint8_t AT24C256RegWriter::i2c_eeprom_read_byte(unsigned int eeaddress) {
	uint8_t rdata = 0xFF;
	Wire.beginTransmission( deviceAddress );
	Wire.write((int)(eeaddress >> 8)); // MSB
	Wire.write((int)(eeaddress & 0xFF)); // LSB
	Wire.endTransmission();
	Wire.requestFrom( deviceAddress, 1 );
	
	if (Wire.available()) 
		rdata = Wire.read();
	
	return rdata;
}

int AT24C256RegWriter::Write(const AccessReg value) {
	byte emptyCode[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int eaddr = findSlot( emptyCode );
	
	if ( eaddr < 0 )
		return -1;

	byte buffer[ PACK_REG_SIZE ];
	AccessRegUtils::AccessReg_pack( value, buffer );

	i2c_eeprom_write_page( eaddr, (byte*)buffer, sizeof( buffer ) );
	return 1;
}

int AT24C256RegWriter::Delete(const byte code[4]) {
	int eaddr = findSlot( code );
	if ( eaddr < 0 )
		return -1;
	
	byte emptyBuffer[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

	i2c_eeprom_write_page( eaddr, (byte*)emptyBuffer, sizeof( emptyBuffer ) );
	return 1;
}

int AT24C256RegWriter::Get(const byte code[4], AccessReg &target) {
	int addr = findSlot( code );

	if ( addr < 0 )
		return -1;

	byte buffer[ PACK_REG_SIZE ];
	for (int i = 0; i < PACK_REG_SIZE; i++)
		buffer[i] = i2c_eeprom_read_byte( i + addr );

	AccessRegUtils::AccessReg_unpack( buffer, target );
	return 1;
}

bool AT24C256RegWriter::Clear() {
	byte emptyBuffer[4] = { 0xFF, 0xFF, 0xFF, 0xFF };


	for ( int i = 0; i < MAX_ADDRESS; i += PACK_REG_SIZE ) 
		if ( i2c_eeprom_read_byte( i ) != 0xFF )
			i2c_eeprom_write_page( i, (byte*)emptyBuffer, sizeof( emptyBuffer ) );

	return true;
}

int AT24C256RegWriter::findSlot( const byte code[4] ){
	for ( int i = 0; i < MAX_ADDRESS; i += PACK_REG_SIZE ) {
		bool found = true;

		for ( int j = i; j < i + 4; j++ ) {
			if ( i2c_eeprom_read_byte( j ) != code[j-i] ) {
				found = false;
				break;
			}
		}
		
		if ( found ) {
			return i;
		}
	}

	return -1;
}

AT24C256RegWriter::~AT24C256RegWriter() { }