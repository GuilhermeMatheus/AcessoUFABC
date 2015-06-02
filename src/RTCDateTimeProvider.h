#ifndef RTCDATETIMEPROVIDER_H
#define RTCDATETIMEPROVIDER_H

#include <Arduino.h>

#include "RTClib.h"
#include "IDateTimeProvider.h"

class RTCDateTimeProvider :
	public IDateTimeProvider {
private:
	RTC_DS1307				*rtc;

public:
	RTCDateTimeProvider( RTC_DS1307* rtc );

	DateTime				GetDateTime();
	String					ToString();

	~RTCDateTimeProvider();
};

#endif /* RTCDATETIMEPROVIDER_H */