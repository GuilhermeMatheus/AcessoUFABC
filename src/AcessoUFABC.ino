#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <RTClib.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>

#include "utils.h"
#include "System.h"
#include "MifareIDProvider.h"
#include "KeyPadListener.h"
#include "LiquidCrystal_I2C.h"
#include "IdleView.h"
#include "RTCDateTimeProvider.h"
#include "NTPDateTimeProvider.h"
#include "MenuPanelView.h"
#include "EepromAccessRegWriter.h"
#include "EepromAccessProvider.h"
#include "NetworkAccessProvider.h"
#include "GuardianKeeper.h"
#include "AT24C256RegWriter.h"

namespace root {
	KeyPadListener						*_keyPadListener;
	MFRC522								*_reader;
	MifareIDProvider					*_mifareIdProvider;
	LiquidCrystal_I2C					*_lcd;
	IdleView							*_idleView;
	RTC_DS1307							*_rtc;
	RTCDateTimeProvider					*_rtcDateTimeProvider;
	NTPDateTimeProvider					*_ntpDateTimeProvider;
	EepromAccessRegWriter				*_eepromAccessRegWriter;
	MenuPanelView						*_menuConfiguration;
	GuardianKeeper						*_guardianKeeper;

	String _converter( byte value[] ) {
		String result = "000000";

		unsigned long sufix = (unsigned int)value[0] + (unsigned int)(256 * value[1]);
		unsigned long prefix = (unsigned long)(unsigned int)value[2] * (unsigned long)100000;
		unsigned long sum = sufix + prefix;

		unsigned long aux = 1000000000;
		while ( sum < aux ) {
			result += "0";
			aux /= 10;
		}

		return result + String(sum, DEC);
	}

	void checkMenuRequest() {
		long menuButtonPressedTime = root::_keyPadListener->GetPressTime( '*', 3000 );

		if (menuButtonPressedTime < 1000)
			return;

		System::LED_GREEN_OFF();
		_menuConfiguration->Active();
		_idleView->ViewChanged();
	}

	void checkCardReader() {
		byte id[4];

		int isCardPresent = _mifareIdProvider->WaitForNextID( id[0], 2000 );
		if (isCardPresent < 0)
			return;

		System::LED_GREEN_OFF();
		_guardianKeeper->AllowAccess( id );
		_idleView->ViewChanged();
	}

	void checkNtpServer( bool checkConstraints ) {
		DateTime now;

		if ( _ntpDateTimeProvider->TryGetDateTime( now, checkConstraints ) ) {
			System::LED_GREEN_OFF();
			System::DT_setDateTime( now, _rtc );
			_idleView->ViewChanged();
		}
	}

}

void setup() {
	root::_lcd =						new LiquidCrystal_I2C ( 0x20,	//I2C address
															    16,		//columns
																2 );	//rows

	ViewBase::InitDisplay( root::_lcd );

	root::_lcd->print( F( "    UFABC" ) );
	root::_lcd->setCursor( 0, 1 );
	root::_lcd->print( F( "  Iniciando..." ) );
	
	Serial.begin( 9600 );

	Wire.begin();
	SPI.begin();

	AT24C256RegWriter *externalWriter = new	AT24C256RegWriter(0x50);	//device address
	root::_eepromAccessRegWriter =		new EepromAccessRegWriter ( externalWriter );

	System::begin( root::_eepromAccessRegWriter );
	
	EthernetUDP *udp =					new EthernetUDP();
	root::_ntpDateTimeProvider =		new NTPDateTimeProvider();

	root::_keyPadListener =				new KeyPadListener ( A3, A4, A5 );
	
	root::_rtc =						new RTC_DS1307 ();
	root::_rtc->begin();

	root::_reader =						new MFRC522 ( 3,				//select pin
													  4 );				//reset power down pin
	root::_reader->PCD_Init();

	root::_mifareIdProvider =			new MifareIDProvider( root::_reader );

	root::_rtcDateTimeProvider =		new RTCDateTimeProvider ( root::_rtc );
	root::_idleView =					new IdleView ( root::_lcd, root::_rtcDateTimeProvider );


	root::_menuConfiguration =			new MenuPanelView ( root::_lcd, 
															root::_keyPadListener, 
															root::_rtc, 
															root::_mifareIdProvider, 
															root::_eepromAccessRegWriter,
															root::_ntpDateTimeProvider );

	EepromAccessProvider *eap =			new EepromAccessProvider( root::_converter, root::_rtcDateTimeProvider );
	NetworkAccessProvider *nap =		new NetworkAccessProvider( root::_converter, eap );
	root::_guardianKeeper =				new GuardianKeeper( nap, root::_lcd );

	if ( System::DT_getUseNTP() )
		root::checkNtpServer( false );

	root::_menuConfiguration->RunSetupIfNeeded();
	root::_idleView->ViewChanged();
}

void loop() {
	System::LED_GREEN_ON();
	
	root::_menuConfiguration->Loop();
	root::_idleView->Draw();

	root::checkMenuRequest();
	root::checkCardReader();
	root::checkNtpServer(true);
}