#include "System.h"
#include <EEPROM.h>

void System::begin() {
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
}

#pragma region Visual
void System::LED_ALL_OFF() {
	System::LED_RED_OFF();
	System::LED_GREEN_OFF();
}

void System::LED_RED_ON() {
	digitalWrite(LED_RED_PIN, HIGH);
}
void System::LED_RED_OFF() {
	digitalWrite(LED_RED_PIN, LOW);
}
void System::LED_RED_BLINK(unsigned long duration) {
	LED_RED_ON();
		delay(duration);
	LED_RED_OFF();
}

void System::LED_GREEN_ON() {
	digitalWrite(LED_GREEN_PIN, HIGH);
}
void System::LED_GREEN_OFF() {
	digitalWrite(LED_GREEN_PIN, LOW);
}
void System::LED_GREEN_BLINK(unsigned long duration) {
	LED_GREEN_ON();
		delay(duration);
	LED_GREEN_OFF();
}

void System::BEEP(unsigned long duration) {
	digitalWrite(BUZZER_PIN, HIGH);
		delay(duration);
	digitalWrite(BUZZER_PIN, LOW);
}
#pragma endregion

#pragma region Access
bool System::ACS_RevokeCard(uint32_t card)
{
	return false;
}
bool System::ACS_AddCard(uint32_t card)
{
	return false;
}

bool System::ACS_RevokeMasterCard(uint32_t card)
{
	return false;
}
bool System::ACS_AddMasterCard(uint32_t card)
{
	return false;
}

uint32_t System::ACS_GetPassword()
{
	return getUInt32Helper(OFFSET_ACS_Password);
}
bool System::ACS_SetPassword(uint32_t password)
{
	return setUInt32Helper(password, OFFSET_ACS_Password);
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

uint32_t System::NW_getIpAddress()
{
	return getUInt32Helper(OFFSET_NW_IpAddress);
}
bool System::NW_setIpAddress(uint32_t value)
{
	return setUInt32Helper(value, OFFSET_NW_IpAddress);
}

uint32_t System::NW_getMask()
{
	return getUInt32Helper(OFFSET_NW_Mask);
}
bool System::NW_setMask(uint32_t value)
{
	return setUInt32Helper(value, OFFSET_NW_Mask);
}

uint32_t System::NW_getGateway()
{
	return getUInt32Helper(OFFSET_NW_Gateway);
}
bool System::NW_setGateway(uint32_t value)
{
	return setUInt32Helper(value, OFFSET_NW_Gateway);
}
#pragma endregion

#pragma region Server 
bool System::SRV_loadIpAddressInto( byte target[4] )
{
	for ( int i = 0; i < 4; i++ )
		target[i] = OFFSET_SRV_IpAddress + i;

	return true;
}
uint32_t  System::SRV_getIpAddress()
{
	return getUInt32Helper(OFFSET_SRV_IpAddress);
}
bool System::SRV_setIpAddress(uint32_t value)
{
	return setUInt32Helper(value, OFFSET_SRV_IpAddress);
}

uint16_t System::SRV_getPort()
{
	uint16_t result = -1;
	eeprom_read_block((void*)&result, (void*)OFFSET_SRV_Port, sizeof(uint16_t));

	return result;
}
bool System::SRV_setPort(uint16_t value)
{
	eeprom_write_block((const void*)&value, (void*)OFFSET_SRV_Port, sizeof(uint16_t));
	return true;
}

uint8_t System::SRV_getComputer()
{
	return EEPROM.read(OFFSET_SRV_Computer);
}
bool System::SRV_setComputer(uint8_t value)
{
	EEPROM.write(OFFSET_SRV_Computer, value);
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
#pragma endregion

#pragma region DateTime 
DateTime System::DT_getDateTime(RTC_DS1307 * rtc)
{
	return rtc->now();
}
bool System::DT_setDateTime(DateTime value, RTC_DS1307 * rtc)
{
	rtc->adjust(value);
	return true;
}

uint32_t System::DT_getNTPIpAddress()
{
	return getUInt32Helper(OFFSET_DT_NTPIpAddress);
}
bool System::DT_setNTPIpAddress(uint32_t value)
{
	return setUInt32Helper(value, OFFSET_DT_NTPIpAddress);
}
#pragma endregion

uint32_t System::getUInt32Helper(int8_t address)
{
	uint32_t result = -1;
	eeprom_read_block((void*)&result, (void*)address, sizeof(uint32_t));

	return result;
}
bool System::setUInt32Helper(uint32_t value, int8_t address)
{
	Serial.print(F("\nEscrevendo em '"));
	Serial.print(address, DEC);
	Serial.print(F("' valor: "));
	Serial.print(value, DEC);

	eeprom_write_block((const void*)&value, (void*)address, sizeof(uint32_t));
	return true;
}