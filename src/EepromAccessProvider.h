#ifndef   EEPROMACCESSPROVIDER_H
#define   EEPROMACCESSPROVIDER_H

#include <Arduino.h>
#include <string.h>

#include "System.h"
#include "AccessReg.h"
#include "AccessProvider.h"
#include "IDateTimeProvider.h"

/**
* Classe respons�vel por verificar a EEPROM e responder requisi��es de acesso.
*/
class EepromAccessProvider :
	public AccessProvider {

private:
	IDateTimeProvider				*dateTimeProvider;

	AccessAttemptResult				RegisterNotFound( byte code[] );
	AccessAttemptResult				CheckConstraints( AccessReg &reg );
	bool							CheckPeriodConstraints( AccessReg &reg );
	void							SetStringSalutation( String &str );

public:
	/**
	* Cria uma inst�ncia de EepromAccessProvider.
	* 
	* @param converter M�todo auxiliar para convers�o do MifareID em uma String codificada.
	* @param dateTimeProvider Dado que {@code AccessReg} possui campos de valida��o de hor�rio, {@code dateTimeProvider} � usado para se obter a data corrente;
	*/
	EepromAccessProvider( String ( *converter ) ( byte[] ), IDateTimeProvider *dateTimeProvider );

	AccessAttemptResult				AllowAccess( byte code[] );

	~EepromAccessProvider();

};

#endif /* EEPROMACCESSPROVIDER_H */