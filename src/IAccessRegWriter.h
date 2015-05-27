#ifndef IACCESSREGWRITER_H
#define IACCESSREGWRITER_H

#include "AccessReg.h"

class IAccessRegWriter
{
public:

	IAccessRegWriter() {

	}
	
	virtual int Write(AccessReg value) = 0;

	virtual ~IAccessRegWriter()
	{
	}

};

#endif /* IACCESSREGWRITER_H */