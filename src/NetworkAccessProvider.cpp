#include "NetworkAccessProvider.h"
#include "utils.h"

void NetworkAccessProvider::begin()
{
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	IPAddress ip(10, 0, 0, 2);
	Ethernet.begin(mac, ip);
}

NetworkAccessProvider::NetworkAccessProvider(String(*converter)(byte&))
	: AccessProvider(converter) { }

int NetworkAccessProvider::AllowAccess( byte& code )
{
	EthernetClient client;
	byte server[] = { 10, 0, 0, 1 };
	uint16_t port = 2050; // System::SRV_getPort();
	//System::SRV_loadIpAddressInto( server );

	if (client.connect(server, port))
	{
		_LOG("connected");

		//21;99;0;1;0000000016533978;E;1;
		String encoded = converter(code);

		String postMessage = "";
		postMessage += controllerPort + ';';
		postMessage += System::SRV_getComputer() + ';';
		postMessage += "0;1;";
		postMessage += converter(code) + ';';
		postMessage += "E;1;" + (char)0x0D;

		_LOGS(encoded);
		client.println(encoded);
	}
	else
	{
		_LOG("connection failed");
	}




	//if (client.connect(server, 80)) {

	//}
	return 1;
	//while (true) {
	//	client = server->available();
	//	String serverResponse = client.readString();
	//	String to = serverResponse.substring(3, 4);

	//	if (to != String(controllerPort, DEC))
	//		continue;
	//	
	//	return serverResponse[13] == 'L' ? 1 : -1;
	//}
}

	
NetworkAccessProvider::~NetworkAccessProvider()
{

}