#ifndef EEPROMACCESSREGWRITER_H
#define EEPROMACCESSREGWRITER_H

#include <EEPROM.h>

#include "System.h"
#include "IAccessRegWriter.h"

class EepromAccessRegWriter :
	public IAccessRegWriter {

private:
	int						findSlot( byte code[4] );

public:
	EepromAccessRegWriter();

	int						Write( AccessReg value );
	int						Delete( byte code[4] );

	~EepromAccessRegWriter();
};

#endif /* EEPROMACCESSREGWRITER_H */