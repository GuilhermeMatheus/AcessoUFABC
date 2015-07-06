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

	EthernetClient client;
	byte server[4];
	System::SRV_loadNTPIpAddressInto( server );
	
	unsigned long time = 0;

	// Just choose any reasonably busy web server, the load is really low
	if ( client.connect( server, 80 ) )
	{
		// Make an HTTP 1.1 request which is missing a Host: header
		// compliant servers are required to answer with an error that includes
		// a Date: header.
		client.print( F( "GET / HTTP/1.1 \r\n\r\n" ) );

		char buf[5];			// temporary buffer for characters
		client.setTimeout( 5000 );
		if ( client.find( ( char* ) "\r\nDate: " )	// look for Date: header
			&& client.readBytes( buf, 5 ) == 5 )	// discard
		{
			unsigned day = client.parseInt();		// day
			client.readBytes(buf, 1);				// discard
			client.readBytes(buf, 3);				// month
			int year = client.parseInt();			// year
			byte hour = client.parseInt();			// hour
			byte minute = client.parseInt();		// minute
			byte second = client.parseInt();		// second

			int daysInPrevMonths;
			switch (buf[0])
			{
			case 'F': daysInPrevMonths = 31; break;		// Feb
			case 'S': daysInPrevMonths = 243; break;	// Sep
			case 'O': daysInPrevMonths = 273; break;	// Oct
			case 'N': daysInPrevMonths = 304; break;	// Nov
			case 'D': daysInPrevMonths = 334; break;	// Dec
			default:
				if (buf[0] == 'J' && buf[1] == 'a')
					daysInPrevMonths = 0;				// Jan
				else if (buf[0] == 'A' && buf[1] == 'p')
					daysInPrevMonths = 90;				// Apr
				else switch (buf[2])
				{
				case 'r': daysInPrevMonths = 59; break;	// Mar
				case 'y': daysInPrevMonths = 120; break;// May
				case 'n': daysInPrevMonths = 151; break;// Jun
				case 'l': daysInPrevMonths = 181; break;// Jul
				default: // add a default label here to avoid compiler warning
				case 'g': daysInPrevMonths = 212; break;// Aug
				}
			}

			// This code will not work after February 2100
			// because it does not account for 2100 not being a leap year and because
			// we use the day variable as accumulator, which would overflow in 2149
			day += (year - 1970) * 365;	// days from 1970 to the whole past year
			day += (year - 1969) >> 2;	// plus one day per leap year 
			day += daysInPrevMonths;	// plus days for previous months this year
			if (daysInPrevMonths >= 59	// if we are past February
				&& ((year & 3) == 0))	// and this is a leap year
				day += 1;			// add one day
			// Remove today, add hours, minutes and seconds this month
			time = (((day - 1ul) * 24 + hour) * 60 + minute) * 60 + second;
		}
	}

	delay( 10 );
	client.flush();
	client.stop();

	if ( time > 0 )
		return DateTime( time );

	return DateTime( SECONDS_FROM_1970_TO_2000 );
}

String NTPDateTimeProvider::ToString() {
	return "na";
}