#include "RTCDateTimeProvider.h"

RTCDateTimeProvider::RTCDateTimeProvider(RTC_DS1307* rtc)
{
	this->rtc = rtc;
}

RTCDateTimeProvider::~RTCDateTimeProvider()
{
}

DateTime RTCDateTimeProvider::GetDateTime()
{
	return rtc->now();
}

String RTCDateTimeProvider::ToString()
{
	const char _barra = '/';
	const char _zero = '0';

	String result = "";
	DateTime now = rtc->now();

	if (now.hour() < 10)
		result.concat(_zero);
	result.concat(now.hour());
	result.concat(':');

	if (now.minute() < 10)
		result.concat(_zero);
	result.concat(now.minute());
	result.concat(' ');

	if (now.day() < 10)
		result.concat(_zero);
	result.concat(now.day());
	result.concat(_barra);

	if (now.month() < 10)
		result.concat(_zero);
	result.concat(now.month());
	result.concat(_barra);

	result.concat(now.year());

	return result;
}