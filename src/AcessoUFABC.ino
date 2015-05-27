#include <EEPROM.h>
#include "System.h"
#include <SPI.h>
#include "RTClib.h"
#include <Wire.h>
#include <MFRC522.h>
#include "MifareIDProvider.h"
#include "KeyPadLongPressListener.h"
#include "KeyPadListener.h"
#include "SpecificEventFunctor.h"
#include "IDateTimeProvider.h"
#include "CompositiveVisual.h"
#include "LiquidCrystal_I2C.h"
#include "LedsVisual.h"
#include "BuzzerVisual.h"
#include "IdleView.h"
#include "RTCDateTimeProvider.h"
#include "MenuPanelView.h"
#include "System.h"
#include "EepromAccessRegWriter.h"
#include <Ethernet.h>
#include "NetworkAccessProvider.h"
#include "utils.h"

#define BUZZER_PIN 5
#define GREEN_LED_PIN 8
#define RED_LED_PIN 7

#define MAX_KEYPAD_FUNCTIONS 2

KeyPadListener* _keyPadListener;
MFRC522* _reader;
MifareIDProvider* _mifareIdProvider;
LiquidCrystal_I2C* _lcd;
IdleView* _idleView;
RTC_DS1307* _rtc;
RTCDateTimeProvider* _rtcDateTimeProvider;
EepromAccessRegWriter* _eepromAccessRegWriter;
MenuPanelView* _menuConfiguration;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	SPI.begin();
	System::begin();
	NetworkAccessProvider::begin();
	
	_reader = new MFRC522(2, 9);
	_reader->PCD_Init();
	_mifareIdProvider = new MifareIDProvider(_reader);

	_keyPadListener = new KeyPadListener(A0, A1, A2, MAX_KEYPAD_FUNCTIONS);

	_lcd = new LiquidCrystal_I2C(0x20, 16, 2);
	_lcd->init();

	_rtc = new RTC_DS1307();
	_rtc->begin();

	_rtcDateTimeProvider = new RTCDateTimeProvider(_rtc);
	_idleView = new IdleView(_lcd, _rtcDateTimeProvider);

	_eepromAccessRegWriter = new EepromAccessRegWriter();

	initConfigurationsMenu();
}

void initConfigurationsMenu()
{
	_menuConfiguration = new MenuPanelView(_lcd, _keyPadListener, _rtc, _mifareIdProvider, _eepromAccessRegWriter);
}

String _converter(byte& value){
	return "dumb";
}

void debugLoop()
{
	NetworkAccessProvider* net = new NetworkAccessProvider(_converter);
	byte id[] = { 1, 2 };
	while (true)
	{
		net->AllowAccess(id[0]);
		_LOG("debugLoop");
		delay(500);
	}
}

void loop()
{
	debugLoop();
	_menuConfiguration->Loop();
	_idleView->Draw();

	if (_keyPadListener->GetPressTime('*', 3000) >= 3000)
		_menuConfiguration->Active();
}

