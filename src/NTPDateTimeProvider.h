#ifndef NTPDATETIMEPROVIDER_H
#define NTPDATETIMEPROVIDER_H

#include <Arduino.h>

#include "IDateTimeProvider.h"

/**
* Classe respons�vel por verificar a data corrente no servidor NTP.
*/
class NTPDateTimeProvider :
	public IDateTimeProvider
{
private:
	uint8_t					backoffExponent;
	long					lastCheckBackoff;
	bool					CheckBackoff();
	bool					CheckConstraints();
public:
	NTPDateTimeProvider();

	bool					TryGetDateTime( DateTime &target, bool checkConstraints = true );
	DateTime				GetDateTime();
	String					ToString();

	~NTPDateTimeProvider();
};

#endif /* NTPDATETIMEPROVIDER_H */