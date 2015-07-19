#include <Ethernet.h>

#include "System.h"
#include "NTPDateTimeProvider.h"

NTPDateTimeProvider::NTPDateTimeProvider() {
	lastCheckBackoff = 0;
	backoffExponent = 0;
}

NTPDateTimeProvider::~NTPDateTimeProvider() { }

bool NTPDateTimeProvider::TryGetDateTime( DateTime &target, bool checkConstraints ) {
	if ( lastCheckBackoff == 0 )
		lastCheckBackoff = millis();

	if ( checkConstraints && !CheckConstraints() )
		return false;

	target = GetDateTime();
	bool result = target.year() > 2000;

	_LOG("Ano");
	_LOGN(target.year(), DEC);
	_LOG("Mes");
	_LOGN(target.month(), DEC);
	_LOG("Dia");
	_LOGN(target.day(), DEC);
	_LOG("Hora");
	_LOGN(target.hour(), DEC);
	_LOG("Min");
	_LOGN(target.minute(), DEC);

	if ( result ) {
		backoffExponent = 27; //2^27 = 134217728 millis = 1,5 days
		lastCheckBackoff = millis();
	}
	else if ( checkConstraints && backoffExponent < 6 )
		backoffExponent = backoffExponent + 1;

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
	EthernetUDP udp;
	static int udpInited = udp.begin(123); // open socket on arbitrary port

	byte target[4];  // NTP server
	System::DT_loadNTPIpAddressInto( target );


	// Only the first four bytes of an outgoing NTP packet need to be set
	// appropriately, the rest can be whatever.
	const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

	// Fail if WiFiUdp.begin() could not init a socket
	if (!udpInited)
		return 0;

	// Clear received data from possible stray received packets
	udp.flush();

	// Send an NTP request
	if (!(udp.beginPacket(target, 123) // 123 is the NTP port
		&& udp.write((byte *)&ntpFirstFourBytes, 48) == 48
		&& udp.endPacket()))
		return 0;				// sending request failed

	// Wait for response; check every pollIntv ms up to maxPoll times
	const int pollIntv = 150;		// poll every this many ms
	const byte maxPoll = 15;		// poll up to this many times
	int pktLen;				// received packet length
	for (byte i = 0; i < maxPoll; i++) {
		if ((pktLen = udp.parsePacket()) == 48)
			break;
		delay(pollIntv);
	}
	if (pktLen != 48)
		return 0;				// no correct packet received

	// Read and discard the first useless bytes
	// Set useless to 32 for speed; set to 40 for accuracy.
	const byte useless = 40;
	for (byte i = 0; i < useless; ++i)
		udp.read();

	// Read the integer part of sending time
	unsigned long time = udp.read();	// NTP time
	for (byte i = 1; i < 4; i++)
		time = time << 8 | udp.read();

	// Round to the nearest second if we want accuracy
	// The fractionary part is the next byte divided by 256: if it is
	// greater than 500ms we round to the next second; we also account
	// for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
	// additionally, we account for how much we delayed reading the packet
	// since its arrival, which we assume on average to be pollIntv/2.
	time += (udp.read() > 115 - pollIntv / 8);

	// Discard the rest of the packet
	udp.flush();

	return DateTime( time - 2208988800ul );		// convert NTP time to Unix time
}


String NTPDateTimeProvider::ToString() {
	return "na";
}