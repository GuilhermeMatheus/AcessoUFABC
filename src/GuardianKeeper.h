#ifndef GUARDIANKEEPER_H
#define GUARDIANKEEPER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "AccessProvider.h"
#include "System.h"
#include "utils.h"

/**
* Classe respons�vel por liberar o acesso.
*/
class GuardianKeeper {
private:
	AccessProvider				*accessProvider;
	LiquidCrystal_I2C			*lcd;
	inline int					RestrictAccess( AccessAttemptResult &result );
	inline int					GiveAccess( AccessAttemptResult &result );

public:
	/**
	* Cria uma inst�ncia de GuardianKeeper.
	* 
	* @param accessProvider O auxiliar a ser consultado a cada requisi��o de acesso.
	* @param lcd O display em que as mensagens ser�o exibidas.
	*/
	GuardianKeeper ( AccessProvider *accessProvider, LiquidCrystal_I2C *lcd );
	
	/**
	* Verifica se o c�digo dado por {@code cardId} possui acesso e mant�m o acesso liberado de acordo com o tempo determinado nas configura��es do sistema.
	* 
	* @param cardId O c�digo mifare de 4 bytes a ser verificado.
	* @return Retorna 1 quando o acesso for liberado ou, caso contr�rio, -1.
	*/
	int							AllowAccess( byte cardId[] );

	~GuardianKeeper();
};

#endif /* GUARDIANKEEPER_H */