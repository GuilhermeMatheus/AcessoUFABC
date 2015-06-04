#ifndef MENUPANELVIEW_H
#define MENUPANELVIEW_H

#define OPTIONS_COUNT	 1
#define KEY_UP			'2'
#define KEY_DOWN		'8'
#define KEY_LEFT		'4'
#define KEY_RIGHT		'6'
#define KEY_OK			'*'
#define KEY_CANCEL		'#'

#include <Arduino.h>

#include "ViewBase.h"
#include "KeyPadListener.h"
#include "IAccessRegWriter.h"
#include "IIDProvider.h"
#include "RTClib.h"

class MenuPanelView :
	public ViewBase {
private:
	const char				Up_Arrow    = ( char )  8;  // 0x0F10;
	const char				Down_Arrow  = ( char )  9;  // 0x0C10;
	const char				Left_Arrow  = ( char ) 18;  // 0x0E10;
	const char				Right_Arrow = ( char ) 19;  // 0x0D10;
	
	bool					hasNewFrame;

protected:
	bool					TryToScroll( int8_t delta );
	void					ListenKeypad();
	void					OnDraw();
	bool					HasNewFrame();

public:
	KeyPadListener			*keyPadListener;
	RTC_DS1307				*rtc;
	IIDProvider				*idProvider;
	IAccessRegWriter		*accessRegWriter;
	
	MenuPanelView( LiquidCrystal_I2C  *lcd, 
				   KeyPadListener	  *keyPadListener,
				   RTC_DS1307		  *rtc,
				   IIDProvider		  *idProvider,
				   IAccessRegWriter   *accessRegWriter );
	
	void					Loop();
	void					Active();
	
	~MenuPanelView();
};

#endif /* MENUPANELVIEW_H */