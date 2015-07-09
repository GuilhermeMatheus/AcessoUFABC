#include "GuardianKeeper.h"

GuardianKeeper::GuardianKeeper( AccessProvider *accessProvider, LiquidCrystal_I2C *lcd ) {
	this->accessProvider = accessProvider;
	this->lcd = lcd;
}

GuardianKeeper::~GuardianKeeper() { }

int GuardianKeeper::AllowAccess( byte cardId[] ) {
	AccessAttemptResult result = accessProvider->AllowAccess( cardId );

	lcd->clear();
	
	lcd->print( result.Response.substring( 0, 16 ) );
	lcd->setCursor( 0, 1 );
	lcd->print( result.Response.substring( 16 ) );
	
	return result.AccessAllowed ? GiveAccess( result ) : RestrictAccess( result );
}

inline int GuardianKeeper::RestrictAccess( AccessAttemptResult &result ) {
	
	System::NOTIFY_ERROR();

	return -1;
}

inline int GuardianKeeper::GiveAccess( AccessAttemptResult &result ) {
	System::LED_GREEN_ON();
	
	System::BEEP( 75 );
	
	delay( 3000 ); //Todo: trocar por System::LigarAcionador

	System::LED_GREEN_OFF();
	
	return 1;
}