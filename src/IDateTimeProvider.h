#ifndef IDATETIMEPROVIDER_H
#define IDATETIMEPROVIDER_H

#include <Arduino.h>

#include "RTClib.h"

class IDateTimeProvider {
public:
	IDateTimeProvider()	{ }

	virtual DateTime		GetDateTime() = 0;
	virtual String			ToString() = 0;

	virtual					~IDateTimeProvider() { }
};

#endif /* IDATETIMEPROVIDER_H */
