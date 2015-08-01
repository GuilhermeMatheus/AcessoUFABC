#ifndef AT24C256REGWRITER_H
#define AT24C256REGWRITER_H

#include <Wire.h>

#include "IAccessRegWriter.h"

#define MAX_ADDRESS 256

class AT24C256RegWriter :
	public IAccessRegWriter
{
	
private:
	int						deviceAddress;
	int						findSlot( const byte code[4] );
	void					i2c_eeprom_write_byte( unsigned int eeaddress, byte data );
	void					i2c_eeprom_write_page( unsigned int eeaddresspage, byte *data, byte length ); // WARNING: address is a page address, 6-bit end will wrap around also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
	uint8_t					i2c_eeprom_read_byte( unsigned int eeaddress );

public:

	AT24C256RegWriter(int deviceAddress);

	int						Write( const AccessReg value );
	int						Delete( const byte code[4] );
	int						Get( const byte code[4], AccessReg &target );
	bool					Clear();

	~AT24C256RegWriter();
};

#endif /* AT24C256REGWRITER_H */