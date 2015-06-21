#ifndef ACCESSPROVIDER_H
#define ACCESSPROVIDER_H

#include <Arduino.h>

struct AccessAttemptResult {
	bool							AccessAllowed;
	char							Response[32];
};

class AccessProvider {

protected:
	String							( *converter )( byte[] );

public:
	AccessProvider ( String ( *converter )( byte[] ) ) {
		this->converter = converter;
	}

	virtual AccessAttemptResult		AllowAccess( byte code[] ) = 0;
	virtual							~AccessProvider() { };
};

#endif /* ACCESSPROVIDER_H */
