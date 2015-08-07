#include "System.h"

IAccessRegWriter *System::accessWriter;

void System::begin(IAccessRegWriter *accessWriter) {
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(GATE_PIN, OUTPUT);

	System::accessWriter = accessWriter;

	CloseGate();

	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	pinMode(53, OUTPUT);
	_LOG("AVR_ATmega");
#endif

	if (System::NW_getIsDHCP()){
		Ethernet.begin(mac);
	}
	else {
		byte ip[4], gateway[4], subnet[4];

		System::NW_loadIpAddressInto(ip);
		System::NW_loadGatewayInto(gateway);
		System::NW_loadMaskInto(subnet);

		Ethernet.begin(mac, ip, gateway, gateway, subnet);
	}

}

#pragma warning( push )
#pragma warning( disable : 4068 ) //warning C4068: unknown pragma

#pragma region Visual
void System::LED_ALL_OFF() {
	System::LED_RED_OFF();
	System::LED_GREEN_OFF();
}

void System::LED_RED_ON() {
	digitalWrite( LED_RED_PIN, HIGH );
}
void System::LED_RED_OFF() {
	digitalWrite( LED_RED_PIN, LOW );
}
void System::LED_RED_BLINK( unsigned long duration ) {
	LED_RED_ON();
		delay( duration );
	LED_RED_OFF();
}

void System::LED_GREEN_ON() {
	digitalWrite( LED_GREEN_PIN, HIGH );
}
void System::LED_GREEN_OFF() {
	digitalWrite( LED_GREEN_PIN, LOW );
}
void System::LED_GREEN_BLINK( unsigned long duration ) {
	LED_GREEN_ON();
		delay(duration);
	LED_GREEN_OFF();
}

void System::BEEP( unsigned long duration ) {
	digitalWrite(BUZZER_PIN, HIGH);
		delay(duration);
	digitalWrite(BUZZER_PIN, LOW);
}

void System::NOTIFY_ERROR( unsigned long duration ) {
	System::LED_RED_ON();

	for ( int i = 0; i < 3; i++ ) {
		System::BEEP( 75 );
		delay(100);
	}

	delay( 2000 );
	System::LED_RED_OFF();

}

#pragma endregion

#pragma region Access
bool System::ACS_RevokeCard( const byte code[] ) {
	return System::accessWriter->Delete( code ) > 0;
}
bool System::ACS_AddAccessReg( const AccessReg &value ) {
	return System::accessWriter->Write( value ) > 0;
}
bool System::ACS_AddMasterCard( const byte code[] ) {
	AccessReg value = AccessReg();

	value.isMaster = true;
	for (int i = 0; i < 4; i++)
		value.mifareID[i] = code[i];

	return System::accessWriter->Write( value ) > 0;
}
uint32_t System::ACS_GetPassword() {
	return getUInt32Helper(OFFSET_ACS_Password);
}
bool System::ACS_SetPassword(uint32_t password) {
	return setUInt32Helper(password, OFFSET_ACS_Password);
}
int System::ACS_GetAccessRegister( const byte mifareID[4], AccessReg &reg  ) {
	return System::accessWriter->Get( mifareID, reg );
}
#pragma endregion

#pragma region Network
bool System::NW_getIsDHCP()
{
	return EEPROM.read(OFFSET_NW_IsDHCP) == 1;
}
bool System::NW_setIsDHCP(bool value)
{
	EEPROM.write(OFFSET_NW_IsDHCP, value ? 1 : 0 );
	return true;
}

bool System::NW_loadIpAddressInto( byte target[4] ){
	return readIpHelper( target, OFFSET_NW_IpAddress );
}
bool System::NW_setIpAddress( const byte value[4] ) {
	return writeIpHelper( value, OFFSET_NW_IpAddress );
}

bool System::NW_loadMaskInto( byte target[4] ) {
	return readIpHelper( target, OFFSET_NW_Mask );
}
bool System::NW_setMask( const byte value[4] ) {
	return writeIpHelper( value, OFFSET_NW_Mask );
}

bool System::NW_loadGatewayInto( byte target[4] ) {
	return readIpHelper( target, OFFSET_NW_Gateway );
}
bool System::NW_setGateway( const byte value[4] ) {
	return writeIpHelper( value, OFFSET_NW_Gateway );
}

uint8_t System::NW_getTerminalNumber()
{
	return EEPROM.read( OFFSET_NW_TerminalNumber );
}
bool System::NW_setTerminalNumber(uint8_t value)
{
	EEPROM.write( OFFSET_NW_TerminalNumber, value );
	return true;
}
#pragma endregion

#pragma region Server 
bool System::SRV_loadIpAddressInto( byte target[4] ) {
	return readIpHelper( target, OFFSET_SRV_IpAddress );
}
bool System::SRV_setIpAddress( const byte value[4] ) {
	return writeIpHelper( value, OFFSET_SRV_IpAddress );
}

uint16_t System::SRV_getPort() {
	uint16_t result = -1;
	eeprom_read_block((void*)&result, (void*)OFFSET_SRV_Port, sizeof(uint16_t));

	return result;
}
bool System::SRV_setPort(uint16_t value) {
	eeprom_write_block((const void*)&value, (void*)OFFSET_SRV_Port, sizeof(uint16_t));
	return true;
}

uint8_t System::SRV_getComputer() {
	return EEPROM.read( OFFSET_SRV_Computer );
}
bool System::SRV_setComputer(uint8_t value) {
	EEPROM.write( OFFSET_SRV_Computer, value );
	return true;
}

#pragma endregion

#pragma region Actuation 
bool System::ACT_getType()
{
	return EEPROM.read(OFFSET_ACT_Type) == 1;
}
bool System::ACT_setType(bool value)
{
	EEPROM.write(OFFSET_ACT_Type, value);
	return true;
}

uint16_t System::ACT_getTime()
{
	uint16_t result = -1;
	eeprom_read_block((void*)&result, (void*)OFFSET_ACT_Time, sizeof(uint16_t));

	return result;
}
bool System::ACT_setTime(uint16_t value)
{
	eeprom_write_block((const void*)&value, (void*)OFFSET_ACT_Time, sizeof(uint16_t));
	return setUInt32Helper(value, true);
}

void System::FreeGate() {
	OpenGate();

	uint16_t delayTime = System::ACT_getTime();
	delay( delayTime );

	CloseGate();
}
void System::CloseGate() {
	digitalWrite( GATE_PIN, System::ACT_getType() ? LOW : HIGH);
}
void System::OpenGate() {
	digitalWrite( GATE_PIN, System::ACT_getType() ? HIGH : LOW);
}

#pragma endregion

#pragma region DateTime 
bool System::DT_loadNTPIpAddressInto( byte target[4] ) {
	return readIpHelper( target, OFFSET_DT_NTPIpAddress );
}
DateTime System::DT_getDateTime(RTC_DS1307 * rtc)
{
	return rtc->now();
}
bool System::DT_setDateTime(DateTime value, RTC_DS1307 * rtc)
{
	rtc->adjust(value);
	return true;
}

bool System::DT_getUseNTP()
{
	return EEPROM.read(OFFSET_DT_UseNTP) == 1;
}
bool System::DT_setUseNTP(bool value)
{
	EEPROM.write(OFFSET_DT_UseNTP, value ? 1 : 0 );
	return true;
}

uint8_t System::DT_getTimeZoneOffset()
{
	return EEPROM.read( OFFSET_DT_TimeZone );
}
bool System::DT_setTimeZoneOffset( uint8_t value )
{
	EEPROM.write( OFFSET_DT_TimeZone, value );
	return true;
}


bool System::DT_getAutoDaylightSaving()
{
	return EEPROM.read(OFFSET_DT_DaylightSaving) == 1;
}
bool System::DT_setAutoDaylightSaving(bool value)
{
	EEPROM.write(OFFSET_DT_DaylightSaving, value ? 1 : 0 );
	return true;
}


uint32_t System::DT_getNTPIpAddress()
{
	return getUInt32Helper(OFFSET_DT_NTPIpAddress);
}
bool System::DT_setNTPIpAddress( const byte ip[4] ) {
	return writeIpHelper( ip, OFFSET_DT_NTPIpAddress );
}
#pragma endregion

bool System::GetIsSetup()
{
	return EEPROM.read(OFFSET_SETUP) == 1;
}
bool System::SetIsSetup()
{
	EEPROM.write(OFFSET_SETUP, 1 );
	return true;
}

bool System::Reset() {
	for ( int i = OFFSET_ACS_Password; i < OFFSET_ACS_Regs; i++ )
		EEPROM.write( i, 0x0 );

	return System::accessWriter->Clear();
}

bool System::writeIpHelper( const byte value[4], int8_t offset ) {
	for ( int i = 0; i < 4; i++ )
		EEPROM.write( offset + i, value[i] );
	
	return true;
}

bool System::readIpHelper( byte target[4], int8_t offset ) {
	for ( int i = 0; i < 4; i++ )
		target[i] = EEPROM.read( offset + i );
	
	return true;
}

uint32_t System::getUInt32Helper( int8_t address )
{
	uint32_t result = -1;
	byte *ptrResult = (byte *)&result;
	for (size_t i = 0; i < sizeof(result); ++i) {
		*ptrResult = EEPROM.read(address);
		++address;
		++ptrResult;
	}		
	return result;
}

bool System::setUInt32Helper(uint32_t value, int8_t address)
{
	_LOGN( value, DEC );

	byte *ptrValue = (byte *)&value;
	for (size_t i = 0; i < sizeof(value); ++i) {
		EEPROM.write(address, *ptrValue);
		++address;
		++ptrValue;
	}		
	return true;
}

#pragma warning( pop ) 