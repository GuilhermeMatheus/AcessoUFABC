#include "EepromAccessProvider.h"

EepromAccessProvider::EepromAccessProvider( String( *converter )( byte[] ), IDateTimeProvider *dateTimeProvider )
	: AccessProvider( converter ) { 
	this->dateTimeProvider = dateTimeProvider;
}

EepromAccessProvider::~EepromAccessProvider() { }

AccessAttemptResult EepromAccessProvider::AllowAccess( byte code[] ) {
	AccessReg reg = System::ACS_GetAccessRegister( code );

	bool notFound = true;
	for (int i = 0; i < 4; i++) {
		if ( reg.mifareID[i] != 0xFF ){
			notFound = false;
			break;
		}
	}
	
	if ( notFound ) {
		AccessAttemptResult result;
		result.AccessAllowed = false;

		strncpy( result.Response, converter( code ).c_str(), 16 );
		strncpy( result.Response + 16, "Acesso Negado", 16 );
		
		return result;
	}

	return CheckConstraints( reg );
}

AccessAttemptResult EepromAccessProvider::CheckConstraints( AccessReg &reg ) {
	AccessAttemptResult result;
	result.AccessAllowed = true;
	StringCopySalutation(result.Response);
	
	if ( reg.isMaster ) {
		strncpy( result.Response + 16, "Master Permitido", 16 );
	}
	else if ( CheckPeriodConstraints( reg ) ) {
		strncpy( result.Response + 16, "Acesso Permitido", 16 );
	}
	else {
		result.AccessAllowed = false;
		strncpy( result.Response + 16, " Acesso Negado  ", 16 );
	}

	return result;
}

void EepromAccessProvider::StringCopySalutation( char *addr ) {
	DateTime dateTime = dateTimeProvider->GetDateTime();

	uint8_t hour = dateTime.hour();
	
	if (hour >= 5 && hour < 12)
		strncpy( addr, "    Bom Dia     ", 16 );
	else if (hour >= 12 && hour < 18)
		strncpy( addr, "   Boa Tarde    ", 16 );
	else
		strncpy( addr, "   Boa Noite    ", 16 );

}
bool EepromAccessProvider::CheckPeriodConstraints( AccessReg &reg ) {
	DateTime dateTime = dateTimeProvider->GetDateTime();

	uint8_t day = dateTime.dayOfWeek();
	uint8_t mask = 1 << day;
	
	if ( (mask & reg.allowedDaysOfWeek) == 0 )
		return false;

	uint8_t hour = dateTime.hour();
	bool inAllowedHour = hour >= reg.hourStart && hour <= reg.hourEnd;
	
	return inAllowedHour;
}