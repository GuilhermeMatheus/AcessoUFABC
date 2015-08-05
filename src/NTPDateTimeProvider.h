#ifndef NTPDATETIMEPROVIDER_H
#define NTPDATETIMEPROVIDER_H

#include <Arduino.h>
#include <Time.h>
#include <EthernetUdp.h>

#include "IDateTimeProvider.h"

//Porta para ouvir os pacotes UDP
#define LOCAL_PORT 8888 

//NTP time stamp is in the first 48 bytes of the message
#define NTP_PACKET_SIZE 48 

/**
* Classe responsável por verificar a data corrente no servidor NTP.
*/
class NTPDateTimeProvider :
	public IDateTimeProvider
{
private:
	EthernetUDP				*Udp;
	byte					packetBuffer[ NTP_PACKET_SIZE ]; 
	uint8_t					backoffExponent;
	long					lastCheckBackoff;
	bool					CheckBackoff();
	bool					CheckConstraints();
	bool					SetInternalTimeAndDate();
	void					SendNTPpacket();

public:
	NTPDateTimeProvider();

	bool					TryGetDateTime( DateTime &target, bool checkConstraints = true );
	DateTime				GetDateTime();
	String					ToString();

	~NTPDateTimeProvider();
};

#endif /* NTPDATETIMEPROVIDER_H */