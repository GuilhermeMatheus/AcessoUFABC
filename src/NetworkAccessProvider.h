#ifndef   NETWORKACCESSPROVIDER_H
#define   NETWORKACCESSPROVIDER_H

#include <Ethernet.h>

#include "AccessProvider.h"
#include "System.h"
#include "utils.h"

class NetworkAccessProvider :
	public AccessProvider {

private:
	EthernetServer					*server;
	AccessProvider					*connectionFallbackProvider;
	int8_t							controllerPort;
	AccessAttemptResult				ParseResponse( String message );
	String							GetPostMessage( byte code[] );
	String							Post( String message );

public:
	NetworkAccessProvider( String ( *converter ) ( byte[] ), AccessProvider *connectionFallbackProvider );

	static void						begin();
	AccessAttemptResult				AllowAccess( byte code[] );

	~NetworkAccessProvider();
};

#endif /* NETWORKACCESSPROVIDER_H */
