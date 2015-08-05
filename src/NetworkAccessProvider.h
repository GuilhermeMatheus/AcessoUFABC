#ifndef   NETWORKACCESSPROVIDER_H
#define   NETWORKACCESSPROVIDER_H

#include <Ethernet.h>

#include "AccessReg.h"
#include "AccessProvider.h"
#include "System.h"
#include "utils.h"

/**
* Classe respons�vel por verificar e responder requisi��es de acesso de acordo com o servidor especificado nas configura��es da controladora.
*/
class NetworkAccessProvider :
	public AccessProvider {

private:
	EthernetServer					*server;
	AccessProvider					*connectionFallbackProvider;
	AccessAttemptResult				ParseResponse( String message );
	String							GetPostMessage( byte code[] );
	String							Post( String message );
	void							SaveResponseInSystemCache( AccessAttemptResult &response, byte code[] );

public:
	/**
	* Cria uma inst�ncia de NetworkAccessProvider.
	* 
	* @param converter M�todo auxiliar para convers�o do MifareID em uma String codificada.
	* @param connectionFallbackProvider Se a conex�o com o servidor falhar, a resposta de acesso ser� dada por {@code connectionFallbackProvider}.
	*/
	NetworkAccessProvider( String ( *converter ) ( byte[] ), AccessProvider *connectionFallbackProvider );

	AccessAttemptResult				AllowAccess( byte code[] );

	~NetworkAccessProvider();
};

#endif /* NETWORKACCESSPROVIDER_H */
