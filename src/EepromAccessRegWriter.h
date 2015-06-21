#ifndef EEPROMACCESSREGWRITER_H
#define EEPROMACCESSREGWRITER_H

#include <EEPROM.h>

#include "System.h"
#include "IAccessRegWriter.h"

class EepromAccessRegWriter :
	public IAccessRegWriter {

private:
	int						findEmptySlot();

public:
	EepromAccessRegWriter();

	int						Write( AccessReg value );

	~EepromAccessRegWriter();
};

#endif /* EEPROMACCESSREGWRITER_H */