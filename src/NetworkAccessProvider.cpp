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

		String encoded = converter(code);

		String postMessage = "";
		postMessage += String(controllerPort, DEC) + ';';
		postMessage += String(System::SRV_getComputer(), DEC) + ';';
		postMessage += "0;1;";
		postMessage += converter(code) + ';';
		postMessage += "E;1;" + String((char)0x0D);

		_LOGS(postMessage);
		client.print(postMessage);
		
		String response = client.readString();
		_LOG("Response: ");
		_LOGS(response);
	}
	else
	{
		_LOG("connection failed");
	}

	return 1;
}

	
NetworkAccessProvider::~NetworkAccessProvider()
{

}