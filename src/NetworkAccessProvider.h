#ifndef   NETWORKACCESSPROVIDER_H
#define   NETWORKACCESSPROVIDER_H

#include <Ethernet.h>

#include "AccessReg.h"
#include "AccessProvider.h"
#include "System.h"
#include "utils.h"

/**
* Classe responsável por verificar e responder requisições de acesso de acordo com o servidor especificado nas configurações da controladora.
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
	* Cria uma instância de NetworkAccessProvider.
	* 
	* @param converter Método auxiliar para conversão do MifareID em uma String codificada.
	* @param connectionFallbackProvider Se a conexão com o servidor falhar, a resposta de acesso será dada por {@code connectionFallbackProvider}.
	*/
	NetworkAccessProvider( String ( *converter ) ( byte[] ), AccessProvider *connectionFallbackProvider );

	AccessAttemptResult				AllowAccess( byte code[] );

	~NetworkAccessProvider();
};

#endif /* NETWORKACCESSPROVIDER_H */
