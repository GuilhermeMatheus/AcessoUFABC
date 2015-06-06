#ifndef   NETWORKACCESSPROVIDER_H
#define   NETWORKACCESSPROVIDER_H

#include <Ethernet.h>

#include "AccessProvider.h"
#include "System.h"

class NetworkAccessProvider :
	public AccessProvider {
private:
	EthernetServer			*server;
	int8_t					controllerPort;

public:
	NetworkAccessProvider(String(*converter)(byte&));

	static void				begin();
	AccessAttemptResult		AllowAccess( byte &code );

	~NetworkAccessProvider();
};

#endif /* NETWORKACCESSPROVIDER_H */
