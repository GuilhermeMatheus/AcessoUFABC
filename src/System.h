#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RTClib.h>
#include <EEPROM.h>

#include "utils.h"
#include "AccessReg.h"
#include "IAccessRegWriter.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MAX_EEPROM_SIZE		  4096
#else
#define MAX_EEPROM_SIZE		  1024
#endif

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

#pragma warning( push )
#pragma warning( disable : 4068) //warning C4068: unknown pragma

class System {
private:
	static IAccessRegWriter		*accessWriter;
	System();
	~System();

public:
	static void					begin( IAccessRegWriter *accessWriter );


/*
===============================================================================
Visual commands.
===============================================================================
*/
#pragma region Visual
	/**
	* Turns off all leds.
	*/
	static void				LED_ALL_OFF();

	/**
	* Turns ON the Red led.
	*/
	static void				LED_RED_ON();
	/**
	* Turns OFF the Red led.
	*/
	static void				LED_RED_OFF();
	/**
	* Turns ON the Red led, pauses the program and then turns OFF the Red led.
	* @param duration The number of milliseconds to pause.
	*/
	static void				LED_RED_BLINK( unsigned long duration = 100 );
	
	/**
	* Turns ON the Red led.
	*/
	static void				LED_GREEN_ON();
	/**
	* Turns OFF the Red led.
	*/
	static void				LED_GREEN_OFF();
	/**
	* Turns ON the Green led, pauses the program and then turns OFF the Green led.
	* @param duration The number of milliseconds to pause.
	*/
	static void				LED_GREEN_BLINK( unsigned long duration = 100 );
	
	/**
	* Turns ON the buzzer, pauses the program and then turns OFF the buzzer.
	* @param duration The number of milliseconds to pause.
	*/
	static void				BEEP( unsigned long duration = 100 );
#pragma endregion


/*
===============================================================================
Access settings.
===============================================================================
*/
#pragma region Access
	static bool				ACS_RevokeCard( byte code[] );
	static bool				ACS_AddAccessReg( AccessReg &value );

	static bool				ACS_RevokeMasterCard( uint32_t card );
	static bool				ACS_AddMasterCard( uint32_t card );

	static uint32_t			ACS_GetPassword();
	static bool				ACS_SetPassword( uint32_t password );
	
	static AccessReg		ACS_GetAccessRegister( byte mifareID[4] );
#pragma endregion


/*
===============================================================================
Network settings.
===============================================================================
*/
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


/*
===============================================================================
Server settings.
===============================================================================
*/
#pragma region Server 
	static bool				SRV_loadIpAddressInto( byte target[4] );

	static uint32_t			SRV_getIpAddress();
	static bool				SRV_setIpAddress( uint32_t value );

	static uint16_t			SRV_getPort();
	static bool				SRV_setPort( uint16_t value );

	static uint8_t			SRV_getComputer();
	static bool				SRV_setComputer( uint8_t value );
#pragma endregion


/*
===============================================================================
Actuation settings.
===============================================================================
*/
#pragma region Actuation 
	static bool				ACT_getType();
	static bool				ACT_setType( bool value );

	static uint16_t			ACT_getTime();
	static bool				ACT_setTime( uint16_t value );
#pragma endregion
	

/*
===============================================================================
DateTime settings.
===============================================================================
*/
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

#pragma warning( pop ) 