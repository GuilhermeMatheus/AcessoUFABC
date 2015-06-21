#ifndef IACCESSREGWRITER_H
#define IACCESSREGWRITER_H

#include <Arduino.h>

#include "AccessReg.h"

class IAccessRegWriter {

public:
	IAccessRegWriter() { }
	
	virtual int				Write( AccessReg value ) = 0;
	virtual int				Delete( byte code[4] ) = 0;

	virtual					~IAccessRegWriter()	{ }
};

#endif /* IACCESSREGWRITER_H */