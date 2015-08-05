#include "NetworkAccessProvider.h"
#include "utils.h"

NetworkAccessProvider::NetworkAccessProvider( String( *converter )( byte[] ), AccessProvider *connectionFallbackProvider )
	: AccessProvider( converter ) {
	this->connectionFallbackProvider = connectionFallbackProvider;
}

NetworkAccessProvider::~NetworkAccessProvider() { }

AccessAttemptResult NetworkAccessProvider::AllowAccess( byte code[] ) {
	String message = GetPostMessage( code );
	String response = Post( message );

	bool connectionFailed = response == "";
	
	if ( connectionFailed ) {
		_LOG("connectionFailed");
		return connectionFallbackProvider->AllowAccess( code );
	}
	else {
		_LOG("connected");
		AccessAttemptResult result = ParseResponse( response );

		SaveResponseInSystemCache( result, code );

		return result;
	}
}

void NetworkAccessProvider::SaveResponseInSystemCache( AccessAttemptResult &response, byte code[] ) {
	System::ACS_RevokeCard( code );
	
	if ( response.AccessAllowed ){
		AccessReg reg;

		reg.isMaster = 0;
		for ( int i = 0; i < 4; i++ )
			reg.mifareID[i] = code[i];
		
		reg.allowedDaysOfWeek = 127;
		reg.hourStart = 0;
		reg.hourEnd = 24;
		reg.untilDay = 31;
		reg.untilMon = 15;
		reg.untilYear = (uint16_t)8192;

		System::ACS_AddAccessReg( reg );
	}
}

String NetworkAccessProvider::GetPostMessage( byte code[] ) {
	String postMessage = String( System::NW_getTerminalNumber(), DEC ) + ';';
	
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

	result.Response = message;
	
	_LOGS( result.Response );

	return result;
}