#ifndef ACCESSPROVIDER_H
#define ACCESSPROVIDER_H

#include <Arduino.h>

/**
* Representa a resposta de verificação de acesso.
*/
struct AccessAttemptResult {
	/**
	* Quando {@code AccessAllowed} for True o acesso foi permitido, caso contrário, seu valor é False.
	*/
	bool							AccessAllowed;
	/**
	* O texto de resposta para a requisição de acesso.
	*/
	String							Response;
};

/**
* Classe responsável por verificar e responder requisições de acesso.
*/
class AccessProvider {

protected:
	String							( *converter )( byte[] );

public:

	/**
	* Cria uma instância de AccessProvider.
	* 
	* @param converter Método auxiliar para conversão do MifareID em uma String codificada.
	*/
	AccessProvider ( String ( *converter )( byte[] ) ) {
		this->converter = converter;
	}

	/**
	* Verifica se o código, após codificado pelo {@code converter} possui acesso ou não.
	* 
	* @param code MifareID de requisição de acesso.
	* @return Retorna a resposta para a requisição de acesso.
	*/
	virtual AccessAttemptResult		AllowAccess( byte code[] ) = 0;
	virtual							~AccessProvider() { };
};

#endif /* ACCESSPROVIDER_H */
