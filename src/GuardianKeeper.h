#ifndef GUARDIANKEEPER_H
#define GUARDIANKEEPER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "AccessProvider.h"
#include "System.h"
#include "utils.h"

/**
* Classe responsável por liberar o acesso.
*/
class GuardianKeeper {
private:
	AccessProvider				*accessProvider;
	LiquidCrystal_I2C			*lcd;
	inline int					RestrictAccess( AccessAttemptResult &result );
	inline int					GiveAccess( AccessAttemptResult &result );

public:
	/**
	* Cria uma instância de GuardianKeeper.
	* 
	* @param accessProvider O auxiliar a ser consultado a cada requisição de acesso.
	* @param lcd O display em que as mensagens serão exibidas.
	*/
	GuardianKeeper ( AccessProvider *accessProvider, LiquidCrystal_I2C *lcd );
	
	/**
	* Verifica se o código dado por {@code cardId} possui acesso e mantém o acesso liberado de acordo com o tempo determinado nas configurações do sistema.
	* 
	* @param cardId O código mifare de 4 bytes a ser verificado.
	* @return Retorna 1 quando o acesso for liberado ou, caso contrário, -1.
	*/
	int							AllowAccess( byte cardId[] );

	~GuardianKeeper();
};

#endif /* GUARDIANKEEPER_H */