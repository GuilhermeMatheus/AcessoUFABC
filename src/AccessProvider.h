#ifndef ACCESSPROVIDER_H
#define ACCESSPROVIDER_H

#include <Arduino.h>

/**
* Representa a resposta de verifica��o de acesso.
*/
struct AccessAttemptResult {
	/**
	* Quando {@code AccessAllowed} for True o acesso foi permitido, caso contr�rio, seu valor � False.
	*/
	bool							AccessAllowed;
	/**
	* O texto de resposta para a requisi��o de acesso.
	*/
	String							Response;
};

/**
* Classe respons�vel por verificar e responder requisi��es de acesso.
*/
class AccessProvider {

protected:
	String							( *converter )( byte[] );

public:

	/**
	* Cria uma inst�ncia de AccessProvider.
	* 
	* @param converter M�todo auxiliar para convers�o do MifareID em uma String codificada.
	*/
	AccessProvider ( String ( *converter )( byte[] ) ) {
		this->converter = converter;
	}

	/**
	* Verifica se o c�digo, ap�s codificado pelo {@code converter} possui acesso ou n�o.
	* 
	* @param code MifareID de requisi��o de acesso.
	* @return Retorna a resposta para a requisi��o de acesso.
	*/
	virtual AccessAttemptResult		AllowAccess( byte code[] ) = 0;
	virtual							~AccessProvider() { };
};

#endif /* ACCESSPROVIDER_H */
