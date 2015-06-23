#ifndef IDATETIMEPROVIDER_H
#define IDATETIMEPROVIDER_H

#include <Arduino.h>

#include "RTClib.h"

/**
* Classe responsável por fornecer a data corrente.
*/
class IDateTimeProvider {
public:
	/**
	* Cria uma instância de IDateTimeProvider
	*/
	IDateTimeProvider()	{ }

	/**
	* Verifica a data corrente.
	* 
	* @return Retorna a data corrente encapsulada em um objeto DateTime.
	*/
	virtual DateTime		GetDateTime() = 0;
	
	/**
	* Verifica a data corrente.
	* 
	* @return Retorna a data corrente formatada em uma String.
	*/
	virtual String			ToString() = 0;

	virtual					~IDateTimeProvider() { }
};

#endif /* IDATETIMEPROVIDER_H */
