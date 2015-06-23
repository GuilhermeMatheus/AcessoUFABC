#ifndef RTCDATETIMEPROVIDER_H
#define RTCDATETIMEPROVIDER_H

#include <Arduino.h>

#include "RTClib.h"
#include "IDateTimeProvider.h"

/**
* Classe responsável por verificar a data corrente no módulo RTC.
*/
class RTCDateTimeProvider :
	public IDateTimeProvider {
private:
	RTC_DS1307				*rtc;

public:
	/**
	* Classe responsável por verificar a data corrente no módulo RTC.
	* 
	* @param rtc Instância do manipulador RTC a ser usado internamente.
	*/
	RTCDateTimeProvider( RTC_DS1307* rtc );

	DateTime				GetDateTime();
	String					ToString();

	~RTCDateTimeProvider();
};

#endif /* RTCDATETIMEPROVIDER_H */