#ifndef   NETWORKACCESSPROVIDER_H
#define   NETWORKACCESSPROVIDER_H

#include "AccessProvider.h"
#include "System.h"

#include <Ethernet.h>

class NetworkAccessProvider :
	public AccessProvider {
private:
	EthernetServer			*server;
	int8_t					controllerPort;

public:
	NetworkAccessProvider( String ( *converter )( byte& ) );

	static void				begin();
	int						AllowAccess( byte &code );

	~NetworkAccessProvider();
};

#endif /* NETWORKACCESSPROVIDER_H */
