#include <Ethernet.h>

#include "System.h"
#include "NTPDateTimeProvider.h"

NTPDateTimeProvider::NTPDateTimeProvider() {
	lastCheckBackoff = 0;
	backoffExponent = 0;

	this->Udp = new EthernetUDP();
	this->Udp->begin( LOCAL_PORT );
}

NTPDateTimeProvider::~NTPDateTimeProvider() { }

bool NTPDateTimeProvider::TryGetDateTime( DateTime &target, bool checkConstraints ) {
	if ( lastCheckBackoff == 0 )
		lastCheckBackoff = millis();

	if ( checkConstraints && !CheckConstraints() )
		return false;

	target = GetDateTime();
	bool result = target.year() > 2000 && target.year() < 2036;

	if (result) { _LOG("NTP ok"); }
	else { _LOG("NTP fail"); }

	if ( result ) {
		backoffExponent = 11; //2^11 * 60000 = 122 880 000 millis = 1.42 days
		lastCheckBackoff = millis();
	}
	
	else if ( checkConstraints && backoffExponent < 6 ) //2^6 * 60000 = 3 840 000 millis = 64 minutes
		backoffExponent += 1;
	
	return result;
}

bool NTPDateTimeProvider::CheckConstraints() {
	if ( !CheckBackoff() )
		return false;

	if ( !System::DT_getUseNTP() )
		return false;
	
	return true;
}

bool NTPDateTimeProvider::CheckBackoff() {
	long now = millis();
	long threshold = lastCheckBackoff + ( 60000 * ( 1 << backoffExponent ) );

	return now > threshold;
}

DateTime NTPDateTimeProvider::GetDateTime() {
	if ( SetInternalTimeAndDate() )
		return DateTime( year(), month(), day(), hour(), minute(), second() );
	
	return DateTime( 0 );
}

bool NTPDateTimeProvider::SetInternalTimeAndDate() {
	SendNTPpacket();

	int maxTries = 20;
	
	while (maxTries > 0) {
		if (Udp->parsePacket() == 48)
			break;

		--maxTries;
		delay(100);
	}

	if ( maxTries == 0 ) {
		_LOG( "No response from the server!" );
	} else {
		Udp->read( packetBuffer, NTP_PACKET_SIZE );
		
		unsigned long highWord, lowWord, epoch;
		
		highWord = word( packetBuffer[40], packetBuffer[41] );
		lowWord = word( packetBuffer[42], packetBuffer[43] );
		epoch = highWord << 16 | lowWord;
		epoch = epoch - 2208988800;
		setTime( epoch );
		
		return true;
	}

	return false;
}

void NTPDateTimeProvider::SendNTPpacket() {
	byte timeServerIP[4] = { 172, 17, 45, 77 };
	//System::DT_loadNTPIpAddressInto( timeServerIP );
	
	IPAddress timeServer( timeServerIP );
	
	// set all bytes in the buffer to 0
	memset( packetBuffer, 0, NTP_PACKET_SIZE );
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = B11100011;   // LI = 11 (clock not sync), Version = 100 (4), Mode = 011 (3 = client)
	packetBuffer[1] = 0;     // Stratum, or type of clock (0 = unspecified)
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp->beginPacket( timeServer, 123 ); //NTP requests are to port 123
	Udp->write( packetBuffer, NTP_PACKET_SIZE );
	Udp->endPacket();
}

String NTPDateTimeProvider::ToString() {
	return "na";
}