#include "EepromAccessProvider.h"

EepromAccessProvider::EepromAccessProvider( String( *converter )( byte[] ), IDateTimeProvider *dateTimeProvider )
	: AccessProvider( converter ) { 
	this->dateTimeProvider = dateTimeProvider;
}

EepromAccessProvider::~EepromAccessProvider() { }

AccessAttemptResult EepromAccessProvider::AllowAccess( byte code[] ) {
	AccessReg reg;
	bool found = System::ACS_GetAccessRegister( code, reg ) > 0;
	
	_LOG("Found:\n	");
	_LOGN(found, BIN);

	_LOG("IsMaster:\n	");
	_LOGN(reg.isMaster, BIN);

	if ( found )
		return CheckConstraints( reg );

	AccessAttemptResult result;
	result.AccessAllowed = false;

	result.Response = converter( code ) + "Acesso Negado";

	return result;
}

AccessAttemptResult EepromAccessProvider::CheckConstraints( AccessReg &reg ) {
	AccessAttemptResult result;
	result.AccessAllowed = true;
	SetStringSalutation( result.Response );
	
	if ( reg.isMaster == 1 ) {
		result.Response += "Master Permitido";
	}
	else if ( CheckPeriodConstraints( reg ) ) {
		result.Response += "Acesso Permitido";
	}
	else {
		result.AccessAllowed = false;
		result.Response += " Acesso Negado";
	}

	return result;
}

void EepromAccessProvider::SetStringSalutation( String &str ) {
	DateTime dateTime = dateTimeProvider->GetDateTime();

	uint8_t hour = dateTime.hour();
	
	if (hour >= 5 && hour < 12)
		str = "    Bom Dia     ";
	else if (hour >= 12 && hour < 18)
		str = "   Boa Tarde    ";
	else
		str = "   Boa Noite    ";

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