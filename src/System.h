#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

#include "RTClib.h"

#define OFFSET_ACS_Password		 0
#define OFFSET_NW_IsDHCP		 4
#define OFFSET_NW_IpAddress		 5
#define OFFSET_NW_Mask			 9 
#define OFFSET_NW_Gateway		13
#define OFFSET_SRV_IpAddress	17
#define OFFSET_SRV_Port			21
#define OFFSET_ACT_Type			23
#define OFFSET_ACT_Time			24
#define OFFSET_DT_NTPIpAddress	26
#define OFFSET_SRV_Computer		30
#define OFFSET_ACS_Regs			31
#define LED_RED_PIN				 8
#define LED_GREEN_PIN			 7
#define BUZZER_PIN				 5

class System
{
private:
	System();
	~System();
public:

	static void				begin();

	#pragma region Visual
	static void				LED_ALL_OFF();

	static void				LED_RED_ON();
	static void				LED_RED_OFF();
	static void				LED_RED_BLINK( unsigned long duration = 100 );
	
	static void				LED_GREEN_ON();
	static void				LED_GREEN_OFF();
	static void				LED_GREEN_BLINK( unsigned long duration = 100 );
	
	static void				BEEP( unsigned long duration = 100 );
	#pragma endregion

	#pragma region Access
	static bool				ACS_RevokeCard( uint32_t card );
	static bool				ACS_AddCard( uint32_t card );

	static bool				ACS_RevokeMasterCard( uint32_t card );
	static bool				ACS_AddMasterCard( uint32_t card );

	static uint32_t			ACS_GetPassword();
	static bool				ACS_SetPassword( uint32_t password );
	#pragma endregion

	#pragma region Network
	static bool				NW_getIsDHCP();
	static bool				NW_setIsDHCP( bool value );
	
	static uint32_t			NW_getIpAddress();
	static bool				NW_setIpAddress( uint32_t value );
	
	static uint32_t			NW_getMask();
	static bool				NW_setMask( uint32_t value );

	static uint32_t			NW_getGateway();
	static bool				NW_setGateway( uint32_t value );
	#pragma endregion

	#pragma region Server 
	static bool				SRV_loadIpAddressInto( byte target[4] );

	static uint32_t			SRV_getIpAddress();
	static bool				SRV_setIpAddress( uint32_t value );

	static uint16_t			SRV_getPort();
	static bool				SRV_setPort( uint16_t value );

	static uint8_t			SRV_getComputer();
	static bool				SRV_setComputer( uint8_t value );
	#pragma endregion

	#pragma region Actuation 
	static bool				ACT_getType();
	static bool				ACT_setType( bool value );

	static uint16_t			ACT_getTime();
	static bool				ACT_setTime( uint16_t value );
	#pragma endregion
	
	#pragma region DateTime 
	static DateTime			DT_getDateTime( RTC_DS1307 * rtc );
	static bool				DT_setDateTime( DateTime value, RTC_DS1307 * rtc );

	static uint32_t			DT_getNTPIpAddress();
	static bool				DT_setNTPIpAddress( uint32_t value );
	#pragma endregion

	static uint32_t			getUInt32Helper( int8_t address );
	static bool				setUInt32Helper( uint32_t value, int8_t address );
};

#endif /* SYSTEM_H */