#ifndef   EEPROMACCESSPROVIDER_H
#define   EEPROMACCESSPROVIDER_H

#include <Arduino.h>
#include <string.h>

#include "System.h"
#include "AccessReg.h"
#include "AccessProvider.h"
#include "IDateTimeProvider.h"

class EepromAccessProvider :
	public AccessProvider {

private:
	IDateTimeProvider				*dateTimeProvider;

	AccessAttemptResult				RegisterNotFound( byte code[] );
	AccessAttemptResult				CheckConstraints( AccessReg &reg );
	bool							CheckPeriodConstraints( AccessReg &reg );
	void							StringCopySalutation( char *addr );

public:
	EepromAccessProvider( String ( *converter ) ( byte[] ), IDateTimeProvider *dateTimeProvider );

	AccessAttemptResult				AllowAccess( byte code[] );

	~EepromAccessProvider();

};

#endif /* EEPROMACCESSPROVIDER_H */