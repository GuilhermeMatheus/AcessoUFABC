#include <Ethernet.h>

#include "System.h"
#include "NTPDateTimeProvider.h"

NTPDateTimeProvider::NTPDateTimeProvider( EthernetUDP *udp ) {
	lastCheckBackoff = 0;
	backoffExponent = 0;

	this->Udp = udp;
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
		backoffExponent = 27; //2^27 = 134217728 millis = 1,5 days
		lastCheckBackoff = millis();
	}
	
	else if ( checkConstraints && backoffExponent < 6 ) 
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
	byte timeServerIP[4];
	System::DT_loadNTPIpAddressInto( timeServerIP );

	IPAddress timeServer( timeServerIP );
	SendNTPpacket( timeServer );

	if ( Udp->parsePacket() ) {
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

void NTPDateTimeProvider::SendNTPpacket( IPAddress &address ) {
	memset( packetBuffer, 0, NTP_PACKET_SIZE );
	packetBuffer[0] = B11100011;
	packetBuffer[1] = 0;
	packetBuffer[2] = 6;
	packetBuffer[3] = 0xEC;
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	Udp->beginPacket( address, 123 );
	Udp->write( (uint8_t*)packetBuffer, NTP_PACKET_SIZE );
	Udp->endPacket();
}

String NTPDateTimeProvider::ToString() {
	return "na";
}