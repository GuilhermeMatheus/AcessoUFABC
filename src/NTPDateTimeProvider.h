#ifndef NTPDATETIMEPROVIDER_H
#define NTPDATETIMEPROVIDER_H

#include <Arduino.h>

#include "IDateTimeProvider.h"

/**
* Classe responsável por verificar a data corrente no servidor NTP.
*/
class NTPDateTimeProvider :
	public IDateTimeProvider
{
public:
	NTPDateTimeProvider();

	bool					TryGetDateTime( DateTime &target );
	DateTime				GetDateTime();
	String					ToString();

	~NTPDateTimeProvider();
};

#endif /* NTPDATETIMEPROVIDER_H */