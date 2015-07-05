#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <RTClib.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Ethernet.h>

#include "utils.h"
#include "System.h"
#include "MifareIDProvider.h"
#include "KeyPadListener.h"
#include "LiquidCrystal_I2C.h"
#include "IdleView.h"
#include "RTCDateTimeProvider.h"
#include "MenuPanelView.h"
#include "EepromAccessRegWriter.h"
#include "EepromAccessProvider.h"
#include "NetworkAccessProvider.h"
#include "GuardianKeeper.h"

namespace root {
	KeyPadListener						*_keyPadListener;
	MFRC522								*_reader;
	MifareIDProvider					*_mifareIdProvider;
	LiquidCrystal_I2C					*_lcd;
	IdleView							*_idleView;
	RTC_DS1307							*_rtc;
	RTCDateTimeProvider					*_rtcDateTimeProvider;
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

	inline void checkMenuRequest() {
		long menuButtonPressedTime = root::_keyPadListener->GetPressTime( '*', 3000 );

		if (menuButtonPressedTime < 1000)
			return;

		_menuConfiguration->Active();
		_idleView->ViewChanged();
	}

	inline void checkCardReader() {
		byte target[4];

		int isCardPresent = _mifareIdProvider->WaitForNextID( target[0], 2000 );
		if (isCardPresent < 0)
			return;

		_guardianKeeper->AllowAccess( target );
		_idleView->ViewChanged();
	}
}

void setup() {
	root::_lcd =						new LiquidCrystal_I2C ( 0x20,	//I2C address
															    16,		//columns
																2 );	//rows

	ViewBase::InitDisplay( root::_lcd );

	root::_lcd->println( F( "    UFABC" ) );
	root::_lcd->setCursor( 0, 1 );
	root::_lcd->println( F( "Starting..." ) );
	
	Serial.begin(9600);

	Wire.begin();
	SPI.begin();

	IAccessRegWriter *earw =			new EepromAccessRegWriter();
	System::begin( earw );
	
	NetworkAccessProvider::begin();

	root::_reader =						new MFRC522 ( 2,				//select pin
													  9 );				//reset power down pin
	root::_reader->PCD_Init();

	root::_mifareIdProvider =			new MifareIDProvider( root::_reader );
	root::_keyPadListener =				new KeyPadListener ( A0, A1, A2 );

	root::_rtc =						new RTC_DS1307 ();
	root::_rtc->begin();

	root::_rtcDateTimeProvider =		new RTCDateTimeProvider ( root::_rtc );
	root::_idleView =					new IdleView ( root::_lcd, root::_rtcDateTimeProvider );
	root::_eepromAccessRegWriter =		new EepromAccessRegWriter ();
	root::_menuConfiguration =			new MenuPanelView ( root::_lcd, 
															root::_keyPadListener, 
															root::_rtc, 
															root::_mifareIdProvider, 
															root::_eepromAccessRegWriter );

	EepromAccessProvider *eap =			new EepromAccessProvider( root::_converter, root::_rtcDateTimeProvider );
	NetworkAccessProvider *nap =		new NetworkAccessProvider( root::_converter, eap );
	root::_guardianKeeper =				new GuardianKeeper( nap, root::_lcd );

	root::_idleView->ViewChanged();

}

void loop() {
	root::_menuConfiguration->Loop();
	root::_idleView->Draw();

	root::checkMenuRequest();
	root::checkCardReader();
}