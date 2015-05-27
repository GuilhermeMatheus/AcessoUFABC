#ifndef ACCESSPROVIDER_H
#define ACCESSPROVIDER_H

#include <Arduino.h>

class AccessProvider
{

protected:
	String(*converter)(byte&);

public:
	AccessProvider(String (*converter)(byte&))
	{
		this->converter = converter;
	}

	virtual int AllowAccess(byte& code) = 0;

	virtual ~AccessProvider() { };
};

#endif /* ACCESSPROVIDER_H */
