#ifndef RTCDATETIMEPROVIDER_H
#define RTCDATETIMEPROVIDER_H

#include <Arduino.h>

#include "RTClib.h"
#include "IDateTimeProvider.h"

/**
* Classe respons�vel por verificar a data corrente no m�dulo RTC.
*/
class RTCDateTimeProvider :
	public IDateTimeProvider {
private:
	RTC_DS1307				*rtc;

public:
	/**
	* Classe respons�vel por verificar a data corrente no m�dulo RTC.
	* 
	* @param rtc Inst�ncia do manipulador RTC a ser usado internamente.
	*/
	RTCDateTimeProvider( RTC_DS1307* rtc );

	DateTime				GetDateTime();
	String					ToString();

	~RTCDateTimeProvider();
};

#endif /* RTCDATETIMEPROVIDER_H */