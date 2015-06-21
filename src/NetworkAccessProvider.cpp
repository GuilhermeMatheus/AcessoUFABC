#include "NetworkAccessProvider.h"
#include "utils.h"

void NetworkAccessProvider::begin()
{
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	IPAddress ip(10, 0, 0, 2);

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	pinMode(53, OUTPUT);
	_LOG("AVR_ATmega");
#endif

	Ethernet.begin(mac, ip);
}

NetworkAccessProvider::NetworkAccessProvider( String( *converter )( byte[] ), AccessProvider *connectionFallbackProvider )
	: AccessProvider( converter ) {
	this->connectionFallbackProvider = connectionFallbackProvider;
}

NetworkAccessProvider::~NetworkAccessProvider() { }

AccessAttemptResult NetworkAccessProvider::AllowAccess( byte code[] ) {
	String message = GetPostMessage( code );
	String response = Post( message );

	bool connectionFailed = response == "";
	
	if ( connectionFailed )
		return connectionFallbackProvider->AllowAccess( code );
	else
		return ParseResponse( response );
}

String NetworkAccessProvider::GetPostMessage( byte code[] ) {
	String postMessage = "";
	postMessage += String( controllerPort, DEC ) + ';';
	postMessage += String( System::SRV_getComputer(), DEC ) + ';';
	postMessage += "0;1;";
	postMessage += converter(code) + ';';
	postMessage += "E;1;" + String((char)0x0D);

	return postMessage;
}

String NetworkAccessProvider::Post( String message ) {
	EthernetClient client;
	byte server[4];
	System::SRV_loadIpAddressInto( server );
	uint16_t port = System::SRV_getPort();
	
	if (client.connect( server, port )) {
		_LOG( "connected" );

		client.print(message);
		
		String serverResponse = client.readString();
		return serverResponse;
	} else {
		_LOG("connection failed");
		return "";
	}
}

AccessAttemptResult NetworkAccessProvider::ParseResponse( String message ) {
	const int	IDX_ACCESS_RESPONSE = 6;
	const char	SEPARATOR = ';';

	AccessAttemptResult result;

	int idx_split = 0;

	for ( int i = 0; i < IDX_ACCESS_RESPONSE; i++ ) {
		message = message.substring( idx_split );
		idx_split = message.indexOf( SEPARATOR ) + 1;
	}

	result.AccessAllowed = message.charAt(0) == 'L';

	for ( int i = 0; i < 2; i++ ) {
		message = message.substring( idx_split );
		idx_split = message.indexOf( SEPARATOR ) + 1;
	}

	message.toCharArray( result.Response, 33 );
	
	_LOGS( result.Response );

	return result;
}
